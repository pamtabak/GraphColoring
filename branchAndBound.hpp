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
	int lowerBoundLimit;
	// pai deste no na arvore de solucao
	struct decisionNode* father;
} decisionNode;

class BranchAndBound
{
public:
	void algorithm (unordered_map<int, unordered_set<int>> graph, int &smallestNumberOfColors, int *bestSolution)
	{
		int numberOfNodes   = graph.size();
		int upperBoundLimit = numberOfNodes;

		decisionNode *firstNode;
		firstNode = (decisionNode*) malloc (sizeof (decisionNode));

		firstNode->node            = 0;
		firstNode->color           = 0;
		firstNode->lowerBoundLimit = graph[0].size(); // degree
		firstNode->father          = NULL;

		vector<decisionNode*> L;
		L.push_back(firstNode);

		while (L.size() > 0)
		{
			decisionNode *l      = chooseNode(L);
			int          *colors = rebuildSolution(l, numberOfNodes);

			int lowerBoundLimit = getLowerLimit(colors, numberOfNodes);
			if (lowerBoundLimit >= smallestNumberOfColors)
			{
				continue;
			}

			// we also need to check if limit is viable
			if (isPromising(colors, l->node, l->color, graph))
			{
				int numberOfNodesWithColor = numberOfNodesThatHaveColor(colors, numberOfNodes);
				// cout << numberOfNodesWithColor << endl;
				if (numberOfNodesWithColor == numberOfNodes)
				{
					int numberOfColorsUsed = getUniqueColors(colors, numberOfNodes);
					if (numberOfColorsUsed < smallestNumberOfColors)
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
						newNode->lowerBoundLimit = lowerBoundLimit;
						newNode->node            = l->node + 1;
						newNode->color           = i;

						L.push_back(newNode);
					}
				}
			}

			delete [] colors;
		}
	}

	// So we can define L(S) = "number of colours used so far" + "number of colours we will be forced to use in the future"
	int getLowerLimit (int* colors, int numberOfNodes)
	{
		// returns number of colors used so far
		// TESTING
		return getUniqueColors(colors, numberOfNodes);
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