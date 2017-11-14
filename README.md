# GraphColoring

Os índices dos vértices devem variar entre 0 e n-1, sendo n o número de vértices do grafo.
O arquivo de entrada deve ser um arquivo txt, em que cada linha representa uma aresta. Ex: 1 2, representa que os nós 1 e 2 estão conectados

Exemplo de como executar o processo:
./main.out "/Users/pamelatabak/Documents/ECI UFRJ/10 periodo/Otimização em Grafos/GraphColoring/Input/test.txt" 0 branchAndBound

## BranchAndBound
#### Limite:
* A cada iteração, o processo calcula o limite inferior em O(n^2). Como:
	* O número de cores usadas até o momento + o número de cores que seremos forçados a usar (calculando em O(n^2))
	* Procuramos por todos os vértices que ainda não foram coloridos. Casos algum desses vértices esteja conectado a vértices com todas as cores usadas até o momento, então precisaremos de pelo menos mais uma cor (já que este vértice não poderá ter uma das cores já usadas). Caso existam dois vértices neste caso citado que são adjacentes, será necessária pelo menos mais uma cor. 
