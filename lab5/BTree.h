#pragma once

#include <vector>
#include <iostream>

class BTree
{
public:
	BTree(int level);
	void insert(int value);
	void print(std::ostream& out);
	bool find(int nodeValue);
private:
	struct Node
	{
		std::vector<int> values;
		std::vector<Node*> sons;
	};

	const int INF = INT_MAX;

	Node* root;
	int level;
	
	std::vector<Node*> copySonsPart(std::vector<Node*> sons, int from, int to);
	std::vector<int> copyValuesPart(std::vector<int> values, int from, int to);
	std::vector<int> insertValue(std::vector<int> values, int value);
	std::pair<int, Node*> recursiveInsert(Node* node, int value);
	bool recursiveFind(Node* node, int nodeValue);
};

