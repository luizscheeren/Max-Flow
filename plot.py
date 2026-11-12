
import re
from pathlib import Path
import matplotlib.pyplot as plt

BASE = Path(__file__).resolve().parent
RESULTS = BASE / "results"
PLOTS = BASE / "plot"
PLOTS.mkdir(exist_ok=True)

# Dimensões: (vértices, arestas), na ordem dos arquivos
CONFIG = {
    "Car": ("resultsCar.txt", [(40, 76), (50, 138), (86, 412)]),
    "Deconvolution": ("resultsDeconv.txt", [(2002, 47872), (2002, 139600)]),
    "Deep LOGISMOS": ("resultsDeep.txt", [
        (340202, 2717499), (989002, 7903455), (669402, 5348653)
    ]),
    "Tsukuba": ("resultsTsukuba.txt", [
        (106850, 532523), (172132, 1085333), (215623, 1488391)
    ])
}

ALGOS = ["Ford-Fulkerson", "EdmondsKarp", "Dinic", "PushRelabel"]
MARKERS = ["o", "s", "^", "d"]
TIMEOUT = 3600 * 1_000_000

for category, (filename, dimensions) in CONFIG.items():

    results = {}
    current_file = current_algo = None

    with open(RESULTS / filename, encoding="utf-8", errors="ignore") as f:
        for line in f:
            line = line.strip()

            if line.startswith("Arquivo:"):
                current_file = line.split(":", 1)[1].strip()
                results[current_file] = {}
            elif line in ALGOS:
                current_algo = line
            elif current_file and current_algo:
                if re.search(r"Status:\s*TIMEOUT", line, re.I):
                    results[current_file][current_algo] = TIMEOUT
                elif match := re.search(r"Average:\s*([\d.]+)", line):
                    results[current_file][current_algo] = min(
                        float(match.group(1)), TIMEOUT
                    )

    data = list(results.values())

    for key, label, suffix in [
        ("edges", "Número de Arestas", "arestas"),
        ("vertices", "Número de Vértices", "vertices"),
        ("density", "Densidade do Grafo", "densidade")
    ]:
        plt.figure(figsize=(11, 6))

        for i, algo in enumerate(ALGOS):
            points = []

            for j, instance in enumerate(data):
                if j >= len(dimensions):
                    break

                vertices, edges = dimensions[j]
                x = {
                    "edges": edges,
                    "vertices": vertices,
                    "density": edges / (vertices * (vertices - 1))
                }[key]

                if algo in instance:
                    points.append((x, instance[algo] / 1_000_000))

            points.sort()

            if points:
                x, y = zip(*points)
                plt.plot(x, y, marker=MARKERS[i], label=algo)

        plt.xlabel(label)
        plt.ylabel("Tempo Médio (segundos)")
        plt.title(f"{category} - Tempo por {label}")
        plt.legend()
        plt.grid(True, linestyle="--", alpha=0.6)
        plt.tight_layout()
        plt.savefig(PLOTS / f"{filename[7:-4].lower()}_tempo_{suffix}.png", dpi=300)
        plt.close()
