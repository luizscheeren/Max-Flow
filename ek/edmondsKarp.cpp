#include <iostream>
#include <stdlib.h>
#include <vector>
#include <chrono>
#include "../edge.cpp"
#include "queue.cpp"

using namespace std;

struct path {
    int origin;
    int idEdge;
};

int edmondsKarp(vector<vector<Edge>>& edges)
{
    int maxFlow = 0;
    int pathFlow = INT_MAX;
    int size = edges.size();
    int sink = size - 1; 

    vector<int> visited(size, 0);
    vector<path> parent(size);

    while (true)
    {
        fill(visited.begin(), visited.end(), 0);

        queue* q = createQueue();
        push(q, 0);
        visited[0] = 1;

        bool foundPath = false;

        while (q->head != NULL)
        {
            int u = pop(q);

            if (u == sink)
            {
                foundPath = true;
                break;
            }

            for (int i = 0; i < (int)edges[u].size(); i++)
            {
                Edge& e = edges[u][i];
                int v = e.destination;

                if (!visited[v] && e.maxCapacity - e.flow > 0)
                {
                    visited[v] = 1;
                    parent[v] = {u, i};
                    push(q, v);
                }
            }
        }

        freeQueue(q);

        if (!foundPath)
            break;

        for (int v = sink; v != 0; v = parent[v].origin)
        {
            int u = parent[v].origin;
            int id = parent[v].idEdge;
            pathFlow = min(pathFlow, edges[u][id].maxCapacity - edges[u][id].flow);
        }

        for (int v = sink; v != 0; v = parent[v].origin)
        {
            int u = parent[v].origin;
            int id = parent[v].idEdge;
            int idTwin = edges[u][id].twin;

            edges[u][id].flow += pathFlow;
            edges[v][idTwin].flow -= pathFlow;
        }

        maxFlow += pathFlow;
    }

    return maxFlow;
}

int main()
{
    vector<vector<Edge>> edges(7);

    // 0=s, 1=a, 2=b, 3=c, 4=d, 5=e, 6=t
    add_edge(edges, 0, 1, 3);
    add_edge(edges, 0, 3, 5);
    add_edge(edges, 1, 2, 1);
    add_edge(edges, 1, 4, 2);
    add_edge(edges, 2, 6, 2);
    add_edge(edges, 3, 4, 4);
    add_edge(edges, 4, 5, 7);
    add_edge(edges, 5, 2, 3);
    add_edge(edges, 5, 6, 7);

    auto start = chrono::high_resolution_clock::now();

    cout << "Max Flow: " << edmondsKarp(edges) << endl;

    auto end = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

    cout << "Execution time: " << duration.count() << " microseconds" << endl;

    return 0;
}