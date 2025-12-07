#pragma once

#include <vector>
#include <iostream>

using Record = std::pair<int, std::string>;

class BTree
{
public:
	BTree(int level);
	void insert(int key, std::string value);
	void printStructure(std::ostream& out);
	void printValues(std::ostream& out);
	bool find(int key);
private:
	struct Node
	{
		std::vector<Record> records;
		std::vector<Node*> sons;
	};

	const int INF = INT_MAX;

	Node* root;
	int level;
	
	std::vector<Node*> copySonsPartly(const std::vector<Node*>& sons, int from, int to);
	std::vector<Record> copyRecordsPartly(const std::vector<Record>& records, int from, int to);
	void insertRecord(std::vector<Record>& records, Record& record);
	std::pair<Record, Node*> recursiveInsert(Node* node, Record& record);
	bool recursiveFind(Node* node, int key);
	void recursivePrintStructure(std::ostream& out, Node* node, int level);
	void recursivePrintValues(std::ostream& out, Node* node);
};

