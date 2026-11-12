#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

typedef struct tagEdge {
    int destination;
    int maxCapacity;
    int flow;

    int twin;

} Edge;

void add_edge(vector<vector<Edge>> &edges, int source, int destination, int maxCapacity) {
    int u = source;
    int v = destination;
    int forwardCapacity = maxCapacity;
    int backwardCapacity = maxCapacity;
    int backwardFlow = maxCapacity;

    if (maxCapacity < 0) {
        u = destination;
        v = source;
        forwardCapacity = -maxCapacity;
        backwardCapacity = 0;
        backwardFlow = 0;
    }

    Edge forward = {v, forwardCapacity, 0, (int)(edges[v].size())};
    Edge backward = {u, backwardCapacity, backwardFlow, (int)(edges[u].size())};
    
    edges[u].push_back(forward);
    edges[v].push_back(backward);
}