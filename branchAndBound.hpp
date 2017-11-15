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

typedef struct decisionNode
{
	int node;
	int color;
	// int lowerBoundLimit;
	// pai deste no na arvore de solucao
	struct decisionNode* father;
} decisionNode;

class BranchAndBound
{
public:
	void algorithm (unordered_map<int, unordered_set<int>> graph, int &smallestNumberOfColors, int *bestSolution)
	{
		int numberOfNodes   = graph.size();

		decisionNode *firstNode;
		firstNode = (decisionNode*) malloc (sizeof (decisionNode));

		firstNode->node            = 0;
		firstNode->color           = 0;
		firstNode->father          = NULL;

		vector<decisionNode*> L;
		L.push_back(firstNode);

		while (L.size() > 0)
		{
			decisionNode *l      = chooseNode(L);
			int          *colors = rebuildSolution(l, numberOfNodes);

			int colorsUsedSoFar = getUniqueColors(colors, numberOfNodes);
			int lowerBoundLimit, upperBoundLimit;
			getLimits(colors, graph, colorsUsedSoFar, lowerBoundLimit, upperBoundLimit);

			// >= ??
			if (lowerBoundLimit >= smallestNumberOfColors)
			{
				continue;
			}

			// cout << lowerBoundLimit << "| smallestNumberOfColors: " << smallestNumberOfColors << endl;

			// we also need to check if limit is viable
			if (isPromising(colors, l->node, l->color, graph))
			{
				// int upperBoundLimit = getUpperLimit(colors, numberOfNodes, colorsUsedSoFar);
				// this is only valid if this solution is promising
				if (upperBoundLimit < smallestNumberOfColors)
				{
					smallestNumberOfColors = upperBoundLimit;
				}

				int numberOfNodesWithColor = numberOfNodesThatHaveColor(colors, numberOfNodes);
				if (numberOfNodesWithColor == numberOfNodes)
				{
					int numberOfColorsUsed = getUniqueColors(colors, numberOfNodes);
					if (numberOfColorsUsed <= smallestNumberOfColors)
					{
						smallestNumberOfColors = numberOfColorsUsed;
						for (int i = 0; i < numberOfNodes; i++)
	            		{
	            	    	bestSolution[i] = colors[i];
	            		}
					}
				}
				else
				{
					for (int i = 0; i < smallestNumberOfColors; i++)
					{
						decisionNode *newNode;
						newNode = (decisionNode*) malloc (sizeof (decisionNode));
						newNode->father          = l;
						newNode->node            = l->node + 1;
						newNode->color           = i;

						L.push_back(newNode);
					}
				}
			}

			delete [] colors;
		}
	}

	// upper limit: "number of colours used so far" + "number of vertices not yet coloured"
	// lower limit: "number of colours used so far" + "number of colours we will be forced to use in the future"
	void getLimits (int* colors, unordered_map<int, unordered_set<int>> graph, int colorsUsedSoFar, int &lowerLimit, int &upperLimit)
	{
		int numberOfNodes = graph.size();
		lowerLimit       = colorsUsedSoFar;
		upperLimit       = colorsUsedSoFar;

		// list with nodes that are not colored yet and are adjacent to nodes
		// covering all colors initially used (colors filled before this funcion was called)
		vector<int> nodesAdjacentAllColors;

		for (int i = 0; i < numberOfNodes; i++)
		{
			// Searching all nodes not yet coloured
			if (colors[i] == -1)
			{
				upperLimit += 1;

				// If this node is adjacent to vertices covering all colors used
				unordered_set<int> uniqueColorsSet;
				int uniqueColors; // amount of different colors i has neighbors with
				for (auto it = graph[i].begin(); it != graph[i].end(); ++it)
				{
					// we only want neighbors already colored
			    	if (colors[*it] == -1)
			    	{
			    		continue;
			    	}

			    	unordered_set<int>::const_iterator got = uniqueColorsSet.find (colors[*it]);
			        if ( got == uniqueColorsSet.end() )
			        {
			            uniqueColors += 1;
			            uniqueColorsSet.insert(colors[*it]);
			        }
				}

				if (uniqueColors == colorsUsedSoFar)
				{
					if (nodesAdjacentAllColors.size() == 0)
					{
						// We only want to increase the lower limit using this once (further nodes could
						// share extra color used here and we are not covering this)
						lowerLimit += 1;
					}
					nodesAdjacentAllColors.push_back(i);
				}
			}
		}

		// if any pair of nodes inside nodesAdjacentAllColors are adjacent, then we need to increase
		// one more color. We then break the algorithm because we don`t want to look for cliques (we want
		// the whole thing to happen in O(n^2))
		for (int i = 0; i < nodesAdjacentAllColors.size(); i++)
		{
			for (int j = i+1; j < nodesAdjacentAllColors.size(); j++)
			{
				unordered_set<int>::const_iterator got = graph[i].find (nodesAdjacentAllColors[j]);
				if ( got != graph[i].end() )
				{
					lowerLimit += 1;
					break;
				}
			}
		}
	}

	decisionNode* chooseNode (vector<decisionNode*> &L)
	{
		// returns last added node
		int          index = L.size() - 1;
		decisionNode *node = L[index];
		L.erase (L.begin() + index);
		return node;
	}

	int* rebuildSolution (decisionNode *l, int numberOfNodes)
	{
		int* colors = new int[numberOfNodes];
		for (int i = 0; i < numberOfNodes; i++)
    	{
    		// color -1: it means that we haven`t treated this node yet
        	colors[i] = -1;
    	}

		decisionNode *node = l;
		while (node->father != NULL)
		{
			colors[node->node] = node->color;
			node               = node->father;
		}

		// when node->father == NULL, it means node is the tree root
		colors[node->node] = node->color;

		// delete node;

		return colors;
	}

	/*  check if is ok to add the color "colorUsed" to node "position"
	*/
	bool isPromising (int colors[], int position, int colorUsed, unordered_map<int, unordered_set<int>> graph)
	{
		for (auto it = graph[position].begin(); it != graph[position].end(); ++it)
		{
			if (colors[*it] == colorUsed)
			{
				return false;
			}
		}

		return true;
	}

	int numberOfNodesThatHaveColor (int* colors, int numberOfNodes)
	{
		int numberOfNodesWithColor = 0;
		for (int i = 0; i < numberOfNodes; i++)
		{
			if (colors[i] >= 0)
			{
				numberOfNodesWithColor ++;
			}
		}

		return numberOfNodesWithColor;
	}

	int getUniqueColors (int array[], int size)
	{
	    int uniqueColors = 0;
	    unordered_set<int> uniqueColorsSet;
	    for (int i = 0; i < size; i++)
	    {
	    	if (array[i] == -1)
	    	{
	    		// it`s not a color. ignore it
	    		continue;
	    	}
	        unordered_set<int>::const_iterator got = uniqueColorsSet.find (array[i]);
	        if ( got == uniqueColorsSet.end() )
	        {
	            uniqueColors += 1;
	            uniqueColorsSet.insert(array[i]);
	        }
	    }

	    return uniqueColors;
	}
};