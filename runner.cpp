#include <iostream>
#include <stdlib.h>
#include <vector>
#include <queue>
#include <stack>
#include <chrono>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <windows.h>

#ifndef EDGE_CPP
#define EDGE_CPP
    #include "edge.cpp"
#endif

#include "pushRelabel.cpp"
#include "dinic.cpp"
#include "edmondsKarp.cpp"
#include "fordFulkerson.cpp"

using namespace std;

bool loadDimacsGraph(const string& filename, vector<vector<Edge>>& edges, int& s, int& t) {
    ifstream file(filename);
    if (!file.is_open()) return false;

    edges.clear();
    s = -1;
    t = -1;
    int numNodes = 0, numEdges = 0;
    char type;

    while (file >> type) {
        if (type == 'c') {
            string comment;
            getline(file, comment);
        } else if (type == 'p') {
            string problem_type;
            file >> problem_type >> numNodes >> numEdges;
            edges.resize(numNodes + 1);
        } else if (type == 'n') {
            int id;
            char role;
            file >> id >> role;
            if (role == 's') s = id;
            else if (role == 't') t = id;
        } else if (type == 'a') {
            int u, v, cap;
            file >> u >> v >> cap;
            add_edge(edges, u, v, cap);
        }
    }
    file.close();
    return true;
}

void runSingleAlgorithm(const string& algo, const string& filename, const string& tempOutputFile) {
    vector<vector<Edge>> edges;
    int s = -1, t = -1;
    if (!loadDimacsGraph(filename, edges, s, t) || s == -1 || t == -1) {
        ofstream out(tempOutputFile);
        out << "ERROR" << endl;
        return;
    }

    auto start = chrono::high_resolution_clock::now();
    long long maxFlow = 0;

    if (algo == "Ford-Fulkerson") maxFlow = fordFulkerson(edges, s, t);
    else if (algo == "EdmondsKarp") maxFlow = edmondsKarp(edges, s, t);
    else if (algo == "Dinic") maxFlow = dinic(edges, s, t);
    else if (algo == "PushRelabel") maxFlow = pushRelabel(edges, s, t);

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start).count();

    ofstream out(tempOutputFile);
    out << maxFlow << " " << duration << endl;
}

void runAlgorithmWithProcessTimeout(
    ostream& out,
    const string& selfExe,
    const string& algoName,
    const string& graphFile,
    DWORD timeoutMs = 3600000) 
{
    out << "\n" << algoName << "\n";
    cout << "\n" << algoName << "\n";

    string tempFile = "temp_result_" + algoName + ".txt";
    string cmd = "\"" + selfExe + "\" --run-worker \"" + algoName + "\" \"" + graphFile + "\" \"" + tempFile + "\"";

    long long maxFlow = 0;
    long long times[3] = {0, 0, 0};

    for (int run = 1; run <= 3; run++) {
        STARTUPINFOA si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        if (!CreateProcessA(NULL, &cmd[0], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            out << " Status: Erro ao iniciar processo.\n";
            cout << " Status: Erro ao iniciar processo.\n";
            return;
        }

        DWORD waitResult = WaitForSingleObject(pi.hProcess, timeoutMs);

        if (waitResult == WAIT_TIMEOUT) {
            TerminateProcess(pi.hProcess, 1);
            WaitForSingleObject(pi.hProcess, INFINITE);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
            DeleteFileA(tempFile.c_str());

            out << " Status: TIMEOUT\n";
            cout << " Status: TIMEOUT\n";
            return;
        }

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        ifstream temp(tempFile);
        string status;
        if (temp >> status) {
            if (status == "ERROR") {
                out << " Status: Erro ao carregar grafo no processo filho.\n";
                cout << " Status: Erro ao carregar grafo no processo filho.\n";
                temp.close();
                DeleteFileA(tempFile.c_str());
                return;
            }

            maxFlow = stoll(status);
            temp >> times[run - 1];
        } else {
            out << " Status: Falha na leitura dos resultados.\n";
            cout << " Status: Falha na leitura dos resultados.\n";
            temp.close();
            DeleteFileA(tempFile.c_str());
            return;
        }

        temp.close();
        DeleteFileA(tempFile.c_str());
    }

    double avg = (times[0] + times[1] + times[2]) / 3.0;

    out << " Max Flow: " << maxFlow << "\n";
    cout << " Max Flow: " << maxFlow << "\n";

    for (int i = 0; i < 3; i++) {
        out << " " << (i + 1) << ": " << times[i] << " microseconds\n";
        cout << " " << (i + 1) << ": " << times[i] << " microseconds\n";
    }

    out << fixed << setprecision(2);
    cout << fixed << setprecision(2);

    out << " Average: " << avg << " microseconds\n";
    cout << " Average: " << avg << " microseconds\n";
}

int main(int argc, char* argv[])
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (argc >= 5 && string(argv[1]) == "--run-worker") {
        runSingleAlgorithm(argv[2], argv[3], argv[4]);
        return 0;
    }

    char currentExePath[MAX_PATH];
    GetModuleFileNameA(NULL, currentExePath, MAX_PATH);

    vector<string> filenames = {
        "dimacs/dimacs_BVZ/BVZ-sawtooth/BVZ-sawtooth13.max"
    };

    string outFileName = "results/results.txt";
    ofstream out(outFileName);

    if (!out.is_open()) {
        return 1;
    }

    for (const auto& file : filenames) {
        out << "========================================\n";
        out << "Arquivo: " << file << "\n";
        out << "========================================\n";

        cout << "========================================\n";
        cout << "Arquivo: " << file << "\n";
        cout << "========================================\n";

        runAlgorithmWithProcessTimeout(out, currentExePath, "Ford-Fulkerson", file, 3600000);
        runAlgorithmWithProcessTimeout(out, currentExePath, "EdmondsKarp",   file, 3600000);
        runAlgorithmWithProcessTimeout(out, currentExePath, "Dinic",         file, 3600000);
        runAlgorithmWithProcessTimeout(out, currentExePath, "PushRelabel",   file, 3600000);

        out << "\n";
        cout << "\n";

        out.flush();
        cout.flush();
    }

    out.close();
    return 0;
}