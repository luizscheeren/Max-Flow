#include <iostream>
#include <stdlib.h>
#include <vector>
#include <queue>
#include <chrono>
#include <fstream>
#include <sstream>
#include <string>

#ifndef EDGE_CPP
#define EDGE_CPP
    #include "edge.cpp"
#endif

#ifndef PATH
#define PATH
    struct path {
        int origin;
        int idEdge;
    };
#endif

using namespace std;

int edmondsKarp(vector<vector<Edge>>& edges, int source, int sink)
{
    int maxFlow = 0;
    int size = edges.size();

    vector<int> visited(size, 0);
    vector<path> parent(size);

    while (true)
    {
        fill(visited.begin(), visited.end(), 0);

        queue<int> q;
        q.push(source);
        visited[source] = 1;

        bool foundPath = false;

        while (!q.empty())
        {
            int u = q.front();
            q.pop();

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
                    q.push(v);
                }
            }
        }

        if (!foundPath)
            break;

        int pathFlow = INT_MAX;

        for (int v = sink; v != source; v = parent[v].origin)
        {
            int u = parent[v].origin;
            int id = parent[v].idEdge;
            pathFlow = min(pathFlow, edges[u][id].maxCapacity - edges[u][id].flow);
        }

        for (int v = sink; v != source; v = parent[v].origin)
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

// int main()
// {
//     ios_base::sync_with_stdio(false);
//     cin.tie(NULL);

//     string filename = "dimacs_deconv/graph3x3.max";
//     ifstream file(filename);

//     if (!file.is_open()) {
//         cerr << "Error opening file: " << filename << endl;
//         return 1;
//     }

//     int numNodes = 0;
//     int numEdges = 0;
//     int s = -1;
//     int t = -1;

//     vector<vector<Edge>> edges;

//     char type;
//     while (file >> type) {
//         if (type == 'c') {
//             string comment;
//             getline(file, comment);
//         } 
//         else if (type == 'p') {
//             string problem_type;
//             file >> problem_type >> numNodes >> numEdges;
//             edges.resize(numNodes + 1);
//         } 
//         else if (type == 'n') {
//             int id;
//             char role;
//             file >> id >> role;
//             if (role == 's') s = id;
//             else if (role == 't') t = id;
//         } 
//         else if (type == 'a') {
//             int u, v, cap;
//             file >> u >> v >> cap;
//             add_edge(edges, u, v, cap);
//         }
//     }

//     file.close();

//     auto start = chrono::high_resolution_clock::now();

//     cout << "Max Flow: " << edmondsKarp(edges, s, t) << endl;

//     auto end = chrono::high_resolution_clock::now();

//     auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

//     cout << "Execution time: " << duration.count() << " microseconds" << endl;

//     return 0;
// }