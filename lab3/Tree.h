#include <istream>
#include <vector>

class Tree
{
public:
	struct TreeNode
	{
		unsigned int level = 0;
		std::string value;
		TreeNode* father = nullptr;
		std::vector<TreeNode*> sons;
	};
	Tree(std::istream& in);
	void getUp();
	void getDown(int sonCount);
	void createNode();
	void renameNode();
	void deleteNode();
	void copyNode();
	void copyInFile();
	void pasteNode();
	int printSons();
	bool inRoot();
	bool emptyClipboard();
	std::string currentNode();
private:
	TreeNode* root = nullptr;
	TreeNode* curNode = this->root;
	TreeNode* clipboard = nullptr;
};


