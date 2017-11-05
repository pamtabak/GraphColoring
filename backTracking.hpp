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

class BackTracking
{
public:
	void algorithm (unordered_map<int, unordered_set<int>> graph, int colors[], int position, int colorToBeUsed, int &smallestNumberOfColors, int *bestSolution)
	{
		int numberOfColors = graph.size();

		// we check if is promising and if the index of the color to be used is greater than the smallest number of colors.
		// since we are attributing colors in order, if we use a color with index x it means that we need x + 1 colors so far
		if (isPromising(colors, position, colorToBeUsed, graph) && colorToBeUsed < smallestNumberOfColors)
		{
			colors[position] = colorToBeUsed;
			if (position == (numberOfColors - 1))
			{
				int colorsUsed = getUniqueColors(colors, numberOfColors);
				if ((colorsUsed < smallestNumberOfColors))
				{
					smallestNumberOfColors = colorsUsed;
					for (int i = 0; i < numberOfColors; i++)
	            	{
	            	    bestSolution[i] = colors[i];
	            	}
	        	}
			}
			else
			{
				for (int k = 1; k < numberOfColors; k++)
				{
					algorithm(graph, colors, position + 1, k, smallestNumberOfColors, bestSolution);
				}
			}
		}
	}

	int getUniqueColors (int array[], int size)
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

	/*  check if is ok to add the color "colorToBeUsed" to node "position"
		since we are adding color to nodes in order, we check for every node that
		has an index <= poisition.
	*/
	bool isPromising (int colors[], int position, int colorToBeUsed, unordered_map<int, unordered_set<int>> graph)
	{
		// get all position`s node neighbors that have index smaller than position number
		for (auto it = graph[position].begin(); it != graph[position].end(); ++it)
		{
			if (*it < position)
			{
				if (colors[*it] == colorToBeUsed)
				{
					return false;
				}
			}
		}
		return true;
	}
};