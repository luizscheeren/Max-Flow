#include <iostream>
#include <stdlib.h>
#include <vector>
#include <queue>
#include <stack>
#include <chrono>
#include "edge.cpp"

using namespace std;

int dinic(vector<vector<Edge>>& edges)
{
    int maxFlow = 0;
    int pathFlow = INT_MAX;
    int size = edges.size();
    int sink = size - 1; 

    while (true)
    {
        vector<int> level(size, -1);
        queue<int> q;
        q.push(0);
        level[0] = 0;

        while (!q.empty())
        {
            int u = q.front();
            q.pop();

            for (int i = 0; i < (int)edges[u].size(); i++)
            {
                Edge& e = edges[u][i];
                int v = e.destination;

                if (level[v] < 0 && e.maxCapacity - e.flow > 0)
                {
                    level[v] = level[u] + 1;
                    q.push(v);
                }
            }
        }

        if (level[sink] < 0)
        {
            break;
        }

        vector<int> start(size, 0);

        while (true)
        {
            pathFlow = INT_MAX;
            stack<int> s;
            s.push(0);

            while (!s.empty())
            {
                int u = s.top();

                if (u == sink)
                {
                    break;
                }

                int i = 0;
                int path = 0;
                for ( i = start[u]; i < (int)edges[u].size(); i++)
                {
                    Edge& e = edges[u][i];
                    int v = e.destination;

                    if (level[v] == level[u] + 1 && e.maxCapacity - e.flow > 0)
                    {
                        s.push(v);
                        pathFlow = min(pathFlow, e.maxCapacity - e.flow);
                        path = 1;
                        break;
                    }
                }
                start[u] = i+1;

                if (!path)
                {
                    s.pop();
                }
            }

            if (s.empty() || s.top() != sink)
                break;

            for (int u = sink; u != 0;)
            {
                int p = s.top();
                s.pop();

                for (Edge& e : edges[p])
                {
                    if (e.destination == u)
                    {
                        e.flow += pathFlow;
                        edges[e.destination][e.twin].flow -= pathFlow;
                        u = p;
                        break;
                    }
                }
            }

            maxFlow += pathFlow;
        }
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

    cout << "Max Flow: " << dinic(edges) << endl;

    auto end = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

    cout << "Execution time: " << duration.count() << " microseconds" << endl;

    return 0;
}