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

using namespace std;

class BruteForce
{
public:
	void algorithm (unordered_map<int, unordered_set<int>> graph, int colors[], int position, int &smallestNumberOfColors, int *bestSolution)
	{
		int numberOfColors = graph.size();

	    if (position == (numberOfColors - 1))
	    {
	        if (isViable(graph, colors))
	        {
	            // get distinct colors
	            int uniqueColors = getUniqueColors(colors, numberOfColors);
	            if (uniqueColors < smallestNumberOfColors)
	            {
	                smallestNumberOfColors = uniqueColors;
	                for (int i = 0; i < numberOfColors; i++)
	                {
	                    bestSolution[i] = colors[i];
	                }
	            }
	        }
	    }
	    else
	    {
	        for (int i = 1; i < numberOfColors; i++)
	        {
	            colors[position+1] = i;
	            algorithm(graph, colors, position+1, smallestNumberOfColors, bestSolution);
	        }
	    }
	}

	int getUniqueColors (int* array, int size)
	{
	    int uniqueColors = 0;
	    unordered_set<int> uniqueColorsSet;
	    for (int i = 0; i < size; i++)
	    {
	        unordered_set<int>::const_iterator got = uniqueColorsSet.find (array[i]);
	        if ( got == uniqueColorsSet.end() )
	        {
	            uniqueColors += 1;
	            uniqueColorsSet.insert(array[i]);
	        }
	    }

	    return uniqueColors;
	}

	bool isViable (unordered_map<int, unordered_set<int>> graph, int *colors)
	{
	    int numberOfColors = graph.size();

	    for (int i = 0; i < numberOfColors; i++)
	    {
	        for (auto it = graph[i].begin(); it != graph[i].end(); ++it)
	        {
	            if (colors[i] == colors[*it])
	            {
	                return false;
	            }
	        }
	    }
	    return true;
	}
};