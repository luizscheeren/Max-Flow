#include <iostream>
#include <stdlib.h>
#include <vector>
#include <queue>
#include <stack>
#include <chrono>
#include <fstream>
#include <sstream>
#include <string>
#include "edge.cpp"

using namespace std;

int blockingFlow(vector<vector<Edge>>& edges, int source, int sink, vector<int>& level, vector<int>& start, int& pathFlow, stack<int>& s)
{
    int u = s.top();

    if (u == sink)
    {
        s.pop();
        return pathFlow ;
    }

    for ( int& i = start[u]; i < (int)edges[u].size(); i++)
    {
        Edge& e = edges[u][i];
        int v = e.destination;

        if (level[v] == level[u] + 1 && e.maxCapacity - e.flow > 0)
        {
            s.push(v);
            int pathF = min(pathFlow, e.maxCapacity - e.flow);
            pathF = blockingFlow(edges, source, sink, level, start, pathF, s);
            if(pathF >= 0)
            {
                e.flow += pathF;
                edges[e.destination][e.twin].flow -= pathF;
                s.pop();
                return pathF;
            }
        }
    }

    s.pop();
    return -1;

}

int dinic(vector<vector<Edge>>& edges, int source, int sink)
{
    int maxFlow = 0;
    int size = edges.size();

    while (true)
    {
        vector<int> level(size, -1);
        queue<int> q;
        q.push(source);
        level[source] = 0;

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

        while (true)
        {
            vector<int> start(size, 0);
            int pathFlow = INT_MAX;
            stack<int> s;
            s.push(source);

            pathFlow = blockingFlow(edges, source, sink, level, start, pathFlow, s);
            if (pathFlow == -1)
                break;

            maxFlow += pathFlow;
        }
    }
    return maxFlow;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string filename = "dimacs_BVZ/teste01.max";
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return 1;
    }

    int numNodes = 0;
    int numEdges = 0;
    int s = -1;
    int t = -1;

    vector<vector<Edge>> edges;

    char type;
    while (file >> type) {
        if (type == 'c') {
            string comment;
            getline(file, comment);
        } 
        else if (type == 'p') {
            string problem_type;
            file >> problem_type >> numNodes >> numEdges;
            edges.resize(numNodes + 1);
        } 
        else if (type == 'n') {
            int id;
            char role;
            file >> id >> role;
            if (role == 's') s = id;
            else if (role == 't') t = id;
        } 
        else if (type == 'a') {
            int u, v, cap;
            file >> u >> v >> cap;
            add_edge(edges, u, v, cap);
        }
    }

    file.close();

    auto start = chrono::high_resolution_clock::now();

    cout << "Max Flow: " << dinic(edges, s, t) << endl;

    auto end = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

    cout << "Execution time: " << duration.count() << " microseconds" << endl;

    return 0;
}