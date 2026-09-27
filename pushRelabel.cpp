#include <iostream>
#include <stdlib.h>
#include <vector>
#include <queue>
#include <stack>
#include <chrono>
#include <fstream>
#include <sstream>
#include <string>

#ifndef EDGE_CPP
#define EDGE_CPP
    #include "edge.cpp"
#endif


using namespace std;

int pushRelabel(vector<vector<Edge>>& edges, int source, int sink)
{
    int maxFlow = 0;
    int size = edges.size();

    vector<int> height(size, 0);
    vector<int> excess(size, 0);
    vector<int> start(size, 0);
    vector<int> queued(size, 0);

    height[source] = size;

    queue<int> q;
 
    for (Edge& e : edges[source])
    {
        e.flow = e.maxCapacity;
        edges[e.destination][e.twin].flow -= e.maxCapacity;
        excess[e.destination] += e.maxCapacity;
        excess[source] -= e.maxCapacity;
        q.push(e.destination);
        queued[e.destination] = 1;
    }

    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        queued[u] = 0;

        int h  = INT_MAX;

        while (excess[u] > 0)
        {
            if (start[u] < (int)edges[u].size())
            {
                Edge& e = edges[u][start[u]];
                if (e.maxCapacity - e.flow > 0 && height[u] == height[e.destination] + 1)
                {
                    int flow = min(excess[u], e.maxCapacity - e.flow);
                    e.flow += flow;
                    edges[e.destination][e.twin].flow -= flow;
                    excess[e.destination] += flow;
                    excess[u] -= flow;

                    if (e.destination != sink && e.destination != source && !queued[e.destination])
                    {
                        q.push(e.destination);
                        queued[e.destination] = 1;
                    }

                    if(e.maxCapacity - e.flow == 0)
                    {
                        start[u]++;
                    }

                    if (excess[u] == 0)
                    {
                        start[u] = 0;
                    }
                }
                else
                {   
                    if (e.maxCapacity - e.flow > 0)
                    {
                        h = min(h, height[e.destination]);
                    }
                    start[u]++;
                }
            }
            else
            {
                height[u] = h + 1;
                h = INT_MAX;
                start[u] = 0;
            }
        }
    }

    for (Edge& e : edges[source])
    {
        maxFlow += e.flow;
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

//     cout << "Max Flow: " << pushRelabel(edges, s, t) << endl;

//     auto end = chrono::high_resolution_clock::now();

//     auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

//     cout << "Execution time: " << duration.count() << " microseconds" << endl;

//     return 0;
// }