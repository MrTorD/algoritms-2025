#include "Tree.h"

#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <conio.h>

Tree::Tree(std::istream& in)
{
	std::string s;
	char c;

	int levelCounter{ 0 };
	int prevLevel{ -1 };
	bool isFile{ false };

	Tree::TreeNode* prevPointer = nullptr; 

	while (!in.eof() && in.good())
	{
		levelCounter = 0;
		isFile = false;
		do 
		{
			in.get(c);

			if (c != '.')
			{
				std::getline(in, s);
				if (c != '!')
					s = c + s;
				else 
					isFile = true;
			}
			else
				levelCounter++;
			
		} while (c == '.');

		Tree::TreeNode* p = new TreeNode;
		p->value = s;
		p->level = levelCounter;
		p->isFile = isFile;

		if (prevLevel == -1)
		{
			p->father = nullptr;
			this->root = prevPointer = this->curNode = p;
			prevLevel = 0;
			continue;
		}

		while (levelCounter < prevLevel)
		{
			prevPointer = prevPointer->father;
			prevLevel = prevPointer->level;
		}

		if (levelCounter == prevLevel)
		{
			p->father = prevPointer->father;
			prevPointer->father->sons.push_back(p);
		} 
		else
		{
			prevPointer->sons.push_back(p);
			p->father = prevPointer;
		}
		prevPointer = p;
		prevLevel = levelCounter;
	}
}

int Tree::printSons()
{
	int i{ 0 };
	for (i; i < this->curNode->sons.size(); i++)
	{
		std::cout << i + 1 << ". Перейти к " << this->curNode->sons.at(i)->value << '\n';
	}
	return i + 1;
}

void Tree::printOut(std::ostream& out, Tree::TreeNode* p, int level)
{
	for (int i = 0; i < level; i++)
	{
		out << '.';
	}
	out << p->value << '\n';
	for (int i = 0; i < p->sons.size(); i++)
	{
		printOut(out, p->sons[i], p->sons[i]->level);
	}
}

void fillLineWithSpaces(int len, int size)
{
	for (int i{ len }; i < size; i++)
	{
		std::cout << ' ';
	}
}

void printBorder(int leftSize, int rightSize)
{
	for (int i{ 0 }; i < leftSize + rightSize; i++)
	{
		std::cout << '*';
	}
	std::cout << '\n';
}

void printLine(std::string leftStr, std::string rightStr, int leftSize, int rightSize, bool isArrow)
{
	std::cout << "*";
	(isArrow) ? std::cout << " -> " : std::cout << "    ";
	std::cout << leftStr;
	fillLineWithSpaces(leftStr.length() + 6, leftSize);
	std::cout << "* " << rightStr;
	fillLineWithSpaces(rightStr.length() + 2, rightSize);
	std::cout << "*\n";
}

void Tree::printMenu(bool isFile)
{
	if (isFile)
		std::cout << "Вы выбрали файл " << this->curNode->value << '\n';
	std::cout << "Нажмите:\n";
	if (isFile)
	{
		std::cout << "R - переименовать\n";
		std::cout << "<- - подняться вверх\n";
		std::cout << "C - скопировать\n";
		std::cout << "X - вырезать\n";
		std::cout << "D - удалить\n";
		return;
	}
	if (!this->inRoot()) std::cout << "<- - подняться вверх\n";
	if (this->curNode->sons.size() != 0) std::cout << "-> - перейти к " << this->curNode->sons.at(curSonIndex)->value << '\n';
	std::cout << "N - cоздать папку\n";
	std::cout << "F - создать файл\n";
	std::cout << "R - переименовать\n";
	if (!this->inRoot())
	{
		std::cout << "C - скопировать\n";
		std::cout << "X - вырезать\n";
		std::cout << "D - удалить\n";
	}
	if (!this->emptyClipboard()) std::cout << "V - вставить\n";
	std::cout << "T - вывести директорию в виде файла\n";
	std::cout << "Esc, q, e - выйти\n";
}

void Tree::printContent(int sNum, int lSize, int rSize)
{
	std::string left;
	std::string right;

	for (int i{ 0 }; i < sNum; i++)
	{
		if (this->curNode->sons.size() >= i + 1 && this->curNode->sons.at(curSonIndex)->sons.size() >= i + 1)
		{
			left = this->curNode->sons.at(i)->value;
			right = this->curNode->sons.at(curSonIndex)->sons.at(i)->value;
		}
		else if (this->curNode->sons.size() >= i + 1)
		{
			left = this->curNode->sons.at(i)->value;
			right = "";
		}
		else if (this->curNode->sons.at(curSonIndex)->sons.size() >= i + 1)
		{
			left = "";
			right = this->curNode->sons.at(curSonIndex)->sons.at(i)->value;
		}

		if (this->curNode->sons.at(curSonIndex)->isFile) 
			right = "Просмотр содержимого файла недоступен";
		printLine(left, right, lSize, rSize, this->curSonIndex == i);
	}
}

void Tree::printScreen()
{
	int lSize = 30;
	int rSize = 60;
	int sNum;

	if (this->curNode->isFile)
	{
		printMenu(true);
		return;
	}
	
	if (this->curNode->sons.size() != 0)
	{
		sNum = (this->curNode->sons.size() > this->curNode->sons.at(curSonIndex)->sons.size())
			? this->curNode->sons.size()
			: this->curNode->sons.at(curSonIndex)->sons.size();
	}
	else 
	{
		sNum = -1;
	}
	std::cout << this->curNode->value << '\n';
	printBorder(lSize, rSize);
	printContent(sNum, lSize, rSize);
	printBorder(lSize, rSize);
	printMenu(false);
}

bool Tree::getUp()
{
	if (this->curNode != this->root)
	{
		this->curNode = this->curNode->father;
		return true;
	}
	return false;
}

bool Tree::getDown(int i)
{
	if (this->curNode->sons.size() >= i) 
	{
		this->curNode = this->curNode->sons.at(i - 1);
		return true;
	}
	return false;
}

bool Tree::inRoot()
{
	return this->curNode == this->root;
}

bool Tree::emptyClipboard()
{
	return this->clipboard == nullptr;
}

std::string Tree::currentNode()
{
	return this->curNode->value;
}

void Tree::createNode(bool isFile)
{
	std::string s;
	(isFile) ? std::cout << "Введите название файла\n" : std::cout << "Введите название папки\n";
	std::cin >> s;
	TreeNode* p = new TreeNode;
	p->value = s;
	p->level = this->curNode->level + 1;
	p->father = this->curNode;
	p->isFile = isFile;
	this->curNode->sons.push_back(p);
}

void Tree::renameNode()
{
	std::string s;
	std::cout << "Введите новое название для " << this->curNode->value << '\n';
	std::cin >> s;
	this->curNode->value = s;
}

void Tree::deleteSons(Tree::TreeNode* p)
{
	for (auto son : p->sons) deleteSons(son);
	delete p;
}

void Tree::deleteNode(bool isClipboard)
{
	TreeNode* p;
	(isClipboard) ? p = this->clipboard : p = this->curNode;

	if (p == this->root) return;

	int i{ 0 };
	for (i; i < p->father->sons.size(); i++)
	{
		if (p->father->sons.at(i) == p) break;
	}

	Tree::TreeNode* father = p->father;
	deleteSons(p);
	if (!isClipboard) this->curNode = father;
	if (father != nullptr) father->sons.erase(father->sons.begin() + i);
}

void Tree::copyNode(bool isCutting)
{
	if (this->curNode != this->root)
	{
		this->clipboard = this->curNode;
		this->isCutting = isCutting;
	}
}

Tree::TreeNode* Tree::copy(Tree::TreeNode* p, Tree::TreeNode* father)
{
	Tree::TreeNode* s = new Tree::TreeNode;
	s->value = p->value;
	s->level = father->level + 1;
	s->isFile = p->isFile;
	s->father = father;
	for (auto son : p->sons)
	{
		s->sons.push_back(copy(son, s));
	}
	return s;
}

void Tree::pasteNode(bool isCutting)
{
	if (this->clipboard == nullptr) return;
	TreeNode* p = copy(this->clipboard, this->curNode);
	p->level = p->father->level + 1;
	this->curNode->sons.push_back(p);
	if (isCutting)
	{
		this->deleteNode(this->clipboard);
		this->isCutting = false;
		this->clipboard = nullptr;
	}
}

void Tree::copyInFile()
{
	std::string s;
	std::cout << "Введите название файла:\n";
	std::cin >> s;
	std::ofstream out(s);
	printOut(out, this->root, 0);
}

enum Arrow
{
	esc = 27,
	arrow = 224,
	up = 72,
	down = 80,
	left = 75,
	right = 77
};

bool Tree::handleButton(int ch)
{
	switch (tolower(ch))
	{
	case esc:
	case 'e':
	case 'q':
		return true;
	case 'r':
		renameNode();
		break;
	case 'c':
		copyNode(false);
		break;
	case 'x':
		copyNode(true);
		break;
	case 'v':
	case 'м':
		pasteNode(this->isCutting);
		break;
	case 'd':
		deleteNode(false);
		break;
	case 'n':
		createNode(false);
		break;
	case 'f':
		createNode(true);
		break;
	case 't':
		copyInFile();
		break;
	case arrow:
		switch (_getch())
		{
		case up:
			if (this->curSonIndex > 0)
				this->curSonIndex--;
			break;
		case down:
			if (this->curNode->sons.size() - 1 > this->curSonIndex)
				this->curSonIndex++;
			break;
		case left:
			if (this->getUp())
				this->curSonIndex = 0;
			break;
		case right:
			if (this->getDown(this->curSonIndex + 1))
				this->curSonIndex = 0;
			break;
		}
	break;
	}
	return false;
}