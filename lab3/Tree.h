#include <istream>
#include <vector>

class Tree
{
public:
	Tree(std::istream& in);
	bool getUp();
	bool getDown(int sonCount);
	void createNode(bool isFile);
	void renameNode();
	void deleteNode(bool isClipboard);
	void copyNode(bool isCutting);
	void copyInFile();
	void pasteNode(bool isCutting);
	void printScreen();
	bool handleButton(int ch);
	int printSons();
	bool inRoot();
	bool emptyClipboard();
	std::string currentNode();
private:
	struct TreeNode
	{
		bool isFile = false;
		unsigned int level = 0;
		std::string value;
		TreeNode* father = nullptr;
		std::vector<TreeNode*> sons;
	};

	int curSonIndex = 0;
	TreeNode* root = nullptr;
	TreeNode* curNode = this->root;
	TreeNode* clipboard = nullptr;
	bool isCutting = false;
	void deleteSons(TreeNode* p);
	void printOut(std::ostream& o, TreeNode* p, int level);
	void printMenu(bool isFile);
	void printContent(int sNum, int lSize, int rSize);
	TreeNode* copy(TreeNode* p, TreeNode* father);
};


