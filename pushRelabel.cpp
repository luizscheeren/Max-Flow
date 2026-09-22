#include <iostream>
#include <stdlib.h>
#include <vector>
#include <queue>
#include <stack>
#include <chrono>
#include "edge.cpp"

using namespace std;

int pushRelabel(vector<vector<Edge>>& edges)
{
    int maxFlow = 0;
    int size = edges.size();
    int sink = size - 1; 

    vector<int> height(size, 0);
    vector<int> excess(size, 0);
    vector<int> current(size, 0);

    height[0] = size;
 
    for (Edge& e : edges[0])
    {
        e.flow = e.maxCapacity;
        edges[e.destination][e.twin].flow -= e.maxCapacity;
        excess[e.destination] += e.maxCapacity;
        excess[0] -= e.maxCapacity;
    }

    queue<int> q;
    for (int i = 1; i < size - 1; i++)
    {
        if (excess[i] > 0)
            q.push(i);
    }

    while (!q.empty())
    {
        int u = q.front();
        q.pop();

        int h  = size;

        while (excess[u] > 0)
        {
            if (current[u] < (int)edges[u].size())
            {
                Edge& e = edges[u][current[u]];
                if (e.maxCapacity - e.flow > 0 && height[u] == height[e.destination] + 1)
                {
                    int flow = min(excess[u], e.maxCapacity - e.flow);
                    e.flow += flow;
                    edges[e.destination][e.twin].flow -= flow;
                    excess[e.destination] += flow;
                    excess[u] -= flow;

                    if (excess[e.destination] > 0 && e.destination != sink && e.destination != 0)
                    {
                        q.push(e.destination);
                    }

                    //h = min(h, height[e.destination]);
                    current[u]++;
                }
                else
                {   
                    if (e.maxCapacity - e.flow > 0)
                    {
                    h = min(h, height[e.destination]);
                    }
                    current[u]++;
                }
            }
            else
            {
                height[u] = h + 1;
                h = size;
                current[u] = 0;
            }
        }
    }

    for (Edge& e : edges[0])
    {
        maxFlow += e.flow;
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

    cout << "Max Flow: " << pushRelabel(edges) << endl;

    auto end = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

    cout << "Execution time: " << duration.count() << " microseconds" << endl;

    return 0;
}