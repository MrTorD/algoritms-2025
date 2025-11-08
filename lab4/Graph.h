#include <vector>

#pragma once
class Graph
{
public:
	Graph(int v);
	void addEdge(int from, int to);
	void DFS(int vershina);
private:
	struct Node
	{
		Node* next;
		unsigned int toNextCost;
	};
	std::vector<Node> NodesList;


	//
	std::vector<std::vector<unsigned int>> costsMatrix;

	//
	int numVershin;
	std::vector<std::vector<int>> *adjLists;
	bool* Visited;
};

