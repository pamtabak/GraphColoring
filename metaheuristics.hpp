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
#include <math.h>

using namespace std;

/*
GRASP : executar X iterações
Para cada iteração:
- fazer algoritmo construtivo baseado num algoritmo guloso
- fazer busca local (trocar cor dos vértices na tentativa de diminuir o número total de cores usado) 
até não dar para diminuir número de cores
*/

class MetaHeuristic
{
public:
	void grasp (unordered_map<int, unordered_set<int>> graph, int &smallestNumberOfColors, int *bestSolution, int numberOfIteractions = 1000)
	{
		int colors[graph.size()] = {};
		for (int i = 0; i < graph.size(); i++)
		{
			// -1 is not a color. it works to identify that
			colors[i] = -1;
		}

		for (int i = 0; i < numberOfIteractions; i++)
		{
			int solution = constructiveAlgorithm(graph, colors);
			if (solution < smallestNumberOfColors)
			{
				smallestNumberOfColors = solution;
				for (int j = 0; j < graph.size(); j++)
				{
					bestSolution[j] = colors[j];
				}
			}
			localSearch (graph, bestSolution, graph.size(), smallestNumberOfColors);
			solution = getUniqueColors(bestSolution, graph.size());
			smallestNumberOfColors = min(smallestNumberOfColors, solution);
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

	int constructiveAlgorithm (unordered_map<int, unordered_set<int>> graph, int *colors)
	{
		vector<int> colorsUsed;

		unordered_set<int> v;
		for(auto kv : graph) {
    		v.insert(kv.first);
		}
		
		// sort nodes by degree
		// key is degree, value is node`s id
		vector<pair<int,int>> items;
		for (auto it = v.begin(); it != v.end(); ++it)
		{
			items.push_back(make_pair(graph[*it].size(),*it));
		}
		sort(items.begin(), items.end());

		while (items.size() != 0)
		{
			// choose node to be colored and remove it from v
			int node  = chooseNode(items);
			int color = colorNode(node, graph, colorsUsed, colors);
			colors[node] = color;
		}

		return colorsUsed.size();
	}

	int chooseNode (vector<pair<int,int>> &items)
	{
		// choose a position from items
		// vector items is sorted by degree, in an ascending way
		// we only choose nodes between the 25% nodes with bigger degree
		int t = ceil(0.5*items.size());
		if (t == items.size())
		{
			t -= 1;
		}
		// numbers between t - items.size() - 1
		int position = (rand() % (items.size() - t)) + t;

		int node = items[position].second;
  		items.erase (items.begin()+position);
  		return node;
	}

	// returns color to be used
	int colorNode (int nodeToBeColored, unordered_map<int, unordered_set<int>> graph, vector<int> &colorsUsed, int *colors)
	{
		if (colorsUsed.size() == 0)
		{
			colorsUsed.push_back(0);
			return 0;
		}

		vector<int> neighborColors;
		for (auto it = graph[nodeToBeColored].begin(); it != graph[nodeToBeColored].end(); ++it)
		{
			if (colors[*it] == -1)
			{
				continue;
			}
			neighborColors.push_back(colors[*it]);
		}

		// now we need to check if there is any color already used that this node doesn`t have
		// a neighbor with
		for (int i = 0; i < colorsUsed.size(); i++)
		{
			ptrdiff_t pos = find(neighborColors.begin(), neighborColors.end(), colorsUsed[i]) - neighborColors.begin();
			if(pos >= neighborColors.size()) {
    			// color not used
    			return colorsUsed[i];
			}
		}

		// if not yet returned, than we need a new color
		// return next color
		int newColor = colorsUsed[colorsUsed.size() - 1] + 1;
		colorsUsed.push_back(newColor);
		return newColor;
	}

	void localSearch (unordered_map<int, unordered_set<int>> graph,int * colors, int numberOfNodes, int bestSolution)
	{
		// Idea taken from article:
		// the local search combines the two cardinality color classes into one and 
		// tries to find a valid color for each violating vertex.
		unordered_set<int> removedColors;
		for (int firstColor = 0; firstColor < bestSolution; firstColor++)
		{
			for (int secondColor = firstColor + 1; secondColor < bestSolution; secondColor++)
			{
				// we need to check if we`ve already treated and removed this color from solution
				unordered_set<int>::const_iterator firstGot  = removedColors.find (firstColor);
				unordered_set<int>::const_iterator secondGot = removedColors.find (secondColor);
				if (firstGot != removedColors.end() || secondGot != removedColors.end())
				{
					continue;
				}

				// basically, all nodes that have colors first or second will have color first
				int localSearchColors[numberOfNodes] = {};
				int tempSearchColors[numberOfNodes]  = {};
				for (int i = 0; i < numberOfNodes; i++)
				{
					if (colors[i] == secondColor)
					{
						localSearchColors[i] = firstColor;
						tempSearchColors[i]  = firstColor;
					}
					else
					{
						localSearchColors[i] = colors[i];
						tempSearchColors[i]  = colors[i];
					}
				}

				// for each vertex that is violating coloring rule
				bool couldFixAllViolations = true;
				for (int i = 0; i < numberOfNodes; i++)
				{
					if (tempSearchColors[i] == firstColor)
					{
						// check if it`s violating rule
						bool violation = false;
						unordered_set<int> neighborColors;
						for (auto it = graph[i].begin(); it != graph[i].end(); ++it)
						{
							neighborColors.insert(tempSearchColors[*it]);
							if (tempSearchColors[*it] == firstColor)
							{
								// Violation!
								violation = true;
							}
						}

						if (violation)
						{
							bool couldBeFixed = false;
							// check if there is any color used (different than secondColor) that can replace this color
							for (int j = 0; j < bestSolution; j++)
							{
								unordered_set<int>::const_iterator removedColorGot  = removedColors.find (j);
								if (j == firstColor || j == secondColor || removedColorGot != removedColors.end())
								{
									continue;
								}

								unordered_set<int>::const_iterator got = neighborColors.find (j);
								if (got == neighborColors.end())
								{
									// this color is available for this node!!
									couldBeFixed        = true;
									tempSearchColors[i] = j;
									break;
								}
							}

							if (couldBeFixed == false)
							{
								couldFixAllViolations = false;
								for (int j = 0; j < numberOfNodes; j++)
								{
									tempSearchColors[j] = localSearchColors[j];
								}
							}
						}
					}
				}

				if (couldFixAllViolations)
				{
					removedColors.insert(secondColor);
					for (int i = 0; i < numberOfNodes; i++)
					{
						colors[i] = tempSearchColors[i];
					}
				}
			}
		}
	}
};