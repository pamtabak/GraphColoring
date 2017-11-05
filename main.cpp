#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <string>
#include <ctime>
#include <set>
#include "bruteForce.hpp"

using namespace std;

// g++-6 main.cpp bruteForce.hpp -o main.out

// ./main.out "/Users/pamelatabak/Documents/ECI UFRJ/10 periodo/Otimização em Grafos/GraphColoring/Input/test.txt" 0

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

unordered_map<int, unordered_set<int>> readGraph (string filePath, bool directed)
{
    unordered_map<int, unordered_set<int>> graph;

    ifstream file(filePath);
    string str;
    while (getline(file, str))
    {
        // Each line is an edge - so there are two nodes per line, separated by whitespace
        vector<string> nodeElements;
        split(str, ' ', std::back_inserter(nodeElements));
        
        int firstNode  = stoi(nodeElements[0]);
        int secondNode = stoi(nodeElements[1]);
        
        unordered_set<int> neighbors = graph[firstNode];
        neighbors.insert(secondNode);
        graph[firstNode] = neighbors;

        if (!directed)
        {
        	neighbors = graph[secondNode];
        	neighbors.insert(firstNode);
        	graph[secondNode] = neighbors;
    	}
    }

    return graph;
}

int main (int argc, char * argv[])
{
    // When the execution has started
    startTime = chrono::high_resolution_clock::now();

    if (argc != 3)
    {
        cout << "Wrong number of parameters." << endl;
        return EXIT_FAILURE;
    }

    string filePath(argv[1]);
    bool directed(stoi(argv[2]));

    unordered_map<int, unordered_set<int>> graph = readGraph(filePath, directed);
    int colors[graph.size()] = {};
    int *bestSolution = new int[graph.size()];

    int smallestNumberOfColors = graph.size() + 1;
    BruteForce bruteForce;
    bruteForce.algorithm(graph, colors, 0, smallestNumberOfColors, bestSolution);

    cout << smallestNumberOfColors << endl;
    for (int i = 0; i < graph.size(); i++)
    {
        cout << bestSolution[i];
    }
    cout << "" << endl;

    delete bestSolution;

    // End of execution
    chrono::high_resolution_clock::time_point endTime = chrono::high_resolution_clock::now();
    chrono::duration<double> endTimeSpan              = chrono::duration_cast<chrono::duration<double> >(endTime - startTime);
    printf("end: %lf secs\n", endTimeSpan.count());

    return 0;
}