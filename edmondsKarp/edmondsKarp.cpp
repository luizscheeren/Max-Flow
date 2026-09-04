#include <iostream>
#include <stdlib.h>
#include <vector>
#include <chrono>
#include "queue.cpp"

using namespace std;

int edmondskarp(const vector<vector<int>>& graph)
{
    int maxFlow = 0;
    int pathFlow = INT_MAX;
    int size = graph.size();
    int sink = size - 1; 

    vector<vector<int>> residualGraph(size, vector<int>(size, 0));
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            residualGraph[i][j] = graph[i][j];
        }
    }

    vector<int> visited(size, 0);

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
            visited[u] = 1;

            for (int v = 0; v < size; v++)
            {
                if (!visited[v] && residualGraph[u][v] > 0)
                {
                    push(q, v);

                    if (v == sink)
                    {
                        foundPath = true;
                        visited[v] = 1;
                        break;
                    }
                }
            }

            if (foundPath)
                break;
        }

        freeQueue(q);

        if (!foundPath)
            break;

        int v = sink;
        while(v != 0)
        {
            for (int u = 0; u < size; u++)
            {
                if (residualGraph[u][v] > 0 && visited[u])
                {
                    pathFlow = min(pathFlow, residualGraph[u][v]);
                    v = u;
                    break;
                }
            }
        }

        // Update the residual graph
        v = sink;
        while(v != 0)
        {
            for (int u = 0; u < size; u++)
            {
                if (residualGraph[u][v] > 0 && visited[u])
                {
                    residualGraph[u][v] -= pathFlow;
                    residualGraph[v][u] += pathFlow;
                    v = u;
                    break;
                }
            }
        }

        maxFlow += pathFlow;
    }

    return maxFlow;
}

int main()
{
    // y -> x
    // 0 = s, 1 = a, 2 = b, 3 = c, 4 = d, 5 = e, 6 = t
    // max_flow = 7
    vector<vector<int>> graph = {
        {0, 3, 0, 5, 0, 0, 0},
        {0, 0, 1, 0, 2, 0, 0},
        {0, 0, 0, 0, 0, 0, 2},
        {0, 0, 0, 0, 4, 0, 0},
        {0, 0, 0, 0, 0, 7, 0},
        {0, 0, 3, 0, 0, 0, 7},
        {0, 0, 0, 0, 0, 0, 0}
    };

    auto start = chrono::high_resolution_clock::now();

    cout << "Max Flow: " << edmondskarp(graph) << endl;

    auto end = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

    cout << "Execution time: " << duration.count() << " microseconds" << endl;

    return 0;
}