#include "ScreenHandler.h"
#include <iostream>
#include "FileManager.h"

ScreenHandler::ScreenHandler(FileManager* mng, int leftSize, int rightSize)
{
	this->mng = mng;
	this->lSize = leftSize;
	this->rSize = rightSize;
}

void FillLineWithSpaces(int len, int size)
{
	for (int i{ len }; i < size; i++)
	{
		std::cout << ' ';
	}
}

void ScreenHandler::PrintBorder()
{
	for (int i{ 0 }; i < lSize + rSize; i++)
	{
		std::cout << borderChar;
	}
	std::cout << '\n';
}

void ScreenHandler::PrintLine(std::string leftStr, std::string rightStr, bool isArrow)
{
	std::cout << "*";
	(isArrow) ? std::cout << " -> " : std::cout << "    ";
	std::cout << leftStr;
	FillLineWithSpaces(leftStr.length() + 6, lSize);
	std::cout << "* " << rightStr;
	FillLineWithSpaces(rightStr.length() + 2, rSize);
	std::cout << "*\n";
}

void ScreenHandler::PrintMenu(bool isFile)
{
	if (isFile)
	{
		std::cout << "Вы выбрали файл " << mng->curNode->value << '\n';
		std::cout << "Нажмите:\n";
		std::cout << "R - переименовать\n";
		std::cout << "<- - подняться вверх\n";
		std::cout << "C - скопировать\n";
		std::cout << "X - вырезать\n";
		std::cout << "D - удалить\n";
		return;
	}

	if (!mng->IsRoot()) std::cout << "<- - подняться вверх\n";
	if (mng->curNode->sons.size() != 0) std::cout << "-> - перейти к " << mng->curNode->sons.at(mng->curSonIndex)->value << '\n';
	std::cout << "N - cоздать папку\n";
	std::cout << "F - создать файл\n";
	std::cout << "R - переименовать\n";

	if (!mng->IsRoot())
	{
		std::cout << "C - скопировать\n";
		std::cout << "X - вырезать\n";
		std::cout << "D - удалить\n";
	}

	if (!mng->EmptyClipboard() && !(mng->clipboard.isCutting && mng->CheckForNesting(mng->curNode, mng->clipboard.p))) 
		std::cout << "V - вставить\n";

	std::cout << "T - вывести директорию в виде файла\n";
	std::cout << "Esc - выйти\n";
}

void ScreenHandler::PrintContent(int sNum)
{
	std::string left;
	std::string right;

	for (int i{ 0 }; i < sNum; i++)
	{
		if (mng->curNode->sons.size() >= i + 1 && mng->curNode->sons.at(mng->curSonIndex)->sons.size() >= i + 1)
		{
			left = mng->curNode->sons.at(i)->value;
			right = mng->curNode->sons.at(mng->curSonIndex)->sons.at(i)->value;
		}
		else if (mng->curNode->sons.size() >= i + 1)
		{
			left = mng->curNode->sons.at(i)->value;
			right = "";
		}
		else if (mng->curNode->sons.at(mng->curSonIndex)->sons.size() >= i + 1)
		{
			left = "";
			right = mng->curNode->sons.at(mng->curSonIndex)->sons.at(i)->value;
		}

		if (mng->curNode->sons.at(mng->curSonIndex)->isFile)
			right = "Просмотр содержимого файла недоступен";
		PrintLine(left, right, mng->curSonIndex == i);
	}
}

void ScreenHandler::PrintScreen()
{
	int sNum;

	if (mng->curNode->isFile)
	{
		PrintMenu(true);
		return;
	}

	if (mng->curNode->sons.size() != 0)
	{
		sNum = (mng->curNode->sons.size() > mng->curNode->sons.at(mng->curSonIndex)->sons.size())
			? mng->curNode->sons.size()
			: mng->curNode->sons.at(mng->curSonIndex)->sons.size();
	}
	else
	{
		sNum = -1;
	}
	std::cout << mng->curNode->value << '\n';
	PrintBorder();
	PrintContent(sNum);
	PrintBorder();
	PrintMenu(false);
}