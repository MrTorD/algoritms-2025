#pragma once

#include <istream>
#include <vector>

class FileManager
{
public:
	struct Node
	{
		bool isFile = false;
		unsigned int level = 0;
		std::string value;
		Node* father = nullptr;
		std::vector<Node*> sons;
	};

	struct Clipboard
	{
		Node* p = nullptr;
		bool isCutting = false;
	};

	Clipboard clipboard;
	Node* curNode = this->root;
	int curSonIndex = 0;

	FileManager(std::istream& in);
	bool HandleButton(int ch);
	bool GetUp();
	bool GetDown(int sonCount);
	void PrintInFile();
	void PasteNode();
	bool CheckForNesting(Node* a, Node* b);
	int PrintSons();
	bool IsRoot();
	bool EmptyClipboard();
private:
	Node* root = nullptr;

	Node* Copy(Node* p, Node* father);
	void CreateNode(bool isFile);
	void RenameNode();
	void DeleteNode(Node* p, bool isCutting);
	void CopyNode(bool isCutting);
	void PrintOut(std::ostream& output, Node* root, int level);
	void DeleteSons(Node* p);
};


