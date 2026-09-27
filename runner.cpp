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

#include "pushRelabel.cpp"
#include "dinic.cpp"
#include "edmondsKarp.cpp"
#include "fordFulkerson.cpp"

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string filename = "dimacs_deconv/graph3x3.max";
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

    // Output the results to a file

    string outFileName = "results.txt";
    ofstream out(outFileName);

    if (!out.is_open()) {
        cerr << "Error opening file: " << outFileName << endl;
        return 1;
    }

    vector<vector<Edge>> edgesCopy = edges;

    // FordFulkerson

    auto start = chrono::high_resolution_clock::now();

    out << "Ford-Fulkerson\n Max Flow: " << fordFulkerson(edgesCopy, s, t) << endl;

    auto end = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

    out << "Execution time: " << duration.count() << " microseconds" << endl;

    // EdmondsKarp

    edgesCopy = edges;
    
    start = chrono::high_resolution_clock::now();

    out << "\nEdmondsKarp\n Max Flow: " << edmondsKarp(edgesCopy, s, t) << endl;

    end = chrono::high_resolution_clock::now();

    duration = chrono::duration_cast<chrono::microseconds>(end - start);

    out << "Execution time: " << duration.count() << " microseconds" << endl;

    // Dinic

    edgesCopy = edges;
    
    start = chrono::high_resolution_clock::now();

    out << "\nDinic\n Max Flow: " << dinic(edgesCopy, s, t) << endl;

    end = chrono::high_resolution_clock::now();

    duration = chrono::duration_cast<chrono::microseconds>(end - start);

    out << "Execution time: " << duration.count() << " microseconds" << endl;

    // PushRelabel

    edgesCopy = edges;
    
    start = chrono::high_resolution_clock::now();

    out << "\nPushRelabel\n Max Flow: " << pushRelabel(edgesCopy, s, t) << endl;

    end = chrono::high_resolution_clock::now();

    duration = chrono::duration_cast<chrono::microseconds>(end - start);

    out << "Execution time: " << duration.count() << " microseconds" << endl;

    out.close();

    return 0;
}