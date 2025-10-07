#pragma once

#include "FileManager.h"
class ScreenHandler
{
private:
	int lSize;
	int rSize;
	FileManager* mng;
	char borderChar = '*';
	void PrintContent(int sNum);
	void PrintMenu(bool isFile);
	void PrintBorder();
	void PrintLine(std::string leftStr, std::string rightStr, bool isArrow);
public:
	ScreenHandler(FileManager* mng, int leftSize, int rightSize);
	void PrintScreen();
};

