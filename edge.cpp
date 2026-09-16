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

void add_edge( vector<vector<Edge>> &edges, int source, int destination, int maxCapacity) {

    Edge forward = {destination, maxCapacity, 0, (int)(edges[destination].size())};
    
    Edge backward = {source, maxCapacity, maxCapacity, (int)(edges[source].size())};
    
    edges[source].push_back(forward);
    edges[destination].push_back(backward);
}