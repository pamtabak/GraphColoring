/*
First Fit algorithm is the easiest and fastest technique of all greedy coloring heuristics. The algorithm sequentially assigns each vertex the lowest legal color. This algorithm has the advantage of being very simple and fast and can be implemented to run in O(n)

Algorithm: allocate the lowest color to the current interval that respects the constraints imposed by the intervals that have been colored. Formally, a coloring of a set of intervals by a function h defined on the set of intervals.
In other-words, an interval I is said to be assigned a color h(I) which is a positive integer.
In the coloring obtained by applying the First-Fit algorithm, if an interval I is assigned a color h(I), then for all colors 1 ≤ i < h(I), there exists an interval I such that I ∩ I = φ and h(I) = i.
The simplicity of First-Fit has made it one of the main choices for the purpose.
*/

/*
Description: This selection heuristic uses a certain selection criterion for choosing the next vertex to be colored.
This strategy is better than the First Fit which simply picks a vertex from an arbitrary order.

Some strategies for selecting the next vertex to be colored have been proposed such as:
Largest degree ordering (LDO): It chooses a vertex with the highest number of neighbors.
Each vertex V contains the number of neighbors as vertex.getNumberOfNeighbors(), the algorithm hold a priority queue of the possible vertexes and chooses the first one.

Saturation degree ordering (SDO): The saturation degree of a vertex is defined as the number of its adjacent differently colored vertices. Intuitively, this heuristic provides a better coloring than LDO as it can be implemented to run in O(n^3)
Each vertex V contains the number of its adjacent differently colored vertices as vertex.getNumberOfDiffColVe(), the algorithm hold a priority queue of the possible vertexes and chooses the first one.

Incidence degree ordering (IDO): A modification of the SDO heuristic is the incidence degree ordering. The incidence degree of a vertex is defined as the number of its adjacent colored vertices. This heuristic can be implemented to run in O(n^2)

IDO+LDO: a modified version of the Largest Degree Ordering (LDO) algorithm by combining it with the Incident Degree Ordering (IDO).
The algorithm works as LDO, but when we found that there are two nodes having the same degree, theIDO was used to choose between them. There are twocriteria for chosen the vertex to be colored: (1) The number of vertices connected to the vertexLDO. (2) The number of colored vertices connected to the vertex IDO.
*/

/*
Algoritmo Welsh e Powell:
1) Ordena vértices de acordo com o grau (decrescente em relação ao grau de cada vértice)
2) Cria n conjuntos de cores (cada cor possui seu conjunto) vazios
3) Associar a cor 1 ao primeiro vértice da lista ordenada e ao próximo vértice da lista não adjacente
a ele, e sucessivamente para cada nó da lista não adjacente a um nó com a cor 1
4) Associar a cor 2 ao próximo vértice da lista ainda sem cor. Sucessivamente associar a cor 2 para o próximo
vértice da lista não adjacente aos vértices com cor 2 e que ainda não está colorido.
5) Continuar este processo até que todos os vértices sejam coloridos.
*/