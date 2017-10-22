#include <iostream>
#include "HashTable.hpp"
#include <unordered_set>
#include <sstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <string>
#include <ctime>

using namespace std;

// g++-6 main.cpp HashTable.hpp -o main.out

// ./main.out "/Users/pamelatabak/Documents/ECI UFRJ/10 periodo/Otimização em Grafos/GraphColoring/Input/test.txt"


chrono::high_resolution_clock::time_point startTime;

template<typename Out>
void split(const std::string &s, char delimiter, Out result) 
{
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delimiter)) {
        *(result++) = item;
    }
}

HashTable<string, unordered_set<string>> readGraph (string filePath, unordered_set<string> &nodes)
{
    HashTable<string, unordered_set<string>> graph;

    ifstream file(filePath);
    string str;
    while (getline(file, str))
    {
        // Each line is an edge - so there are two nodes per line, separated by whitespace
        vector<string> nodeElements;
        split(str, ' ', std::back_inserter(nodeElements));
        unordered_set<string> neighbors = graph.get(nodeElements[0]);
        neighbors.insert(nodeElements[1]);
        graph.set(nodeElements[0], neighbors);

        // Listing all different nodes
        nodes.insert(nodeElements[0]);
        nodes.insert(nodeElements[1]);
    }

    return graph;
}

int main (int argc, char * argv[])
{
    // When the execution has started
    startTime = chrono::high_resolution_clock::now();

    if (argc != 2)
    {
        cout << "Wrong number of parameters." << endl;
        return EXIT_FAILURE;
    }

    string filePath(argv[1]);

    // All different nodes that appear on the graph
    unordered_set<string> nodes;
    // All edges (since it`s an unidirectional graph, we only save the edge once)
    HashTable<string, unordered_set<string>> graph = readGraph(filePath, nodes);

    // End of execution
    chrono::high_resolution_clock::time_point endTime = chrono::high_resolution_clock::now();
    chrono::duration<double> endTimeSpan              = chrono::duration_cast<chrono::duration<double> >(endTime - startTime);
    printf("end: %lf secs\n", endTimeSpan.count());

    return 0;
}