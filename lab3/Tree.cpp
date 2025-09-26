#include "Tree.h"

#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

Tree::Tree(std::istream& in)
{
	std::string s;
	char c;

	int levelCounter{ 0 };
	int prevLevel{ -1 };

	Tree::TreeNode* prevPointer = nullptr; 

	while (!in.eof() && in.good())
	{
		levelCounter = 0;
		do 
		{
			in.get(c);

			if (c != '.')
			{
				std::getline(in, s);
				s = c + s;
			}
			else
				levelCounter++;
			
		} while (c == '.');

		Tree::TreeNode* p = new TreeNode;
		p->value = s;
		p->level = levelCounter;

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

void printOut(std::ostream& out, Tree::TreeNode* p, int level)
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

void Tree::getUp()
{
	if (this->curNode != this->root) this->curNode = this->curNode->father;
}

void Tree::getDown(int i)
{
	if (this->curNode->sons.size() >= i) this->curNode = this->curNode->sons.at(i - 1);
}

bool Tree::inRoot()
{
	return (this->curNode == this->root);
}

bool Tree::emptyClipboard()
{
	return (this->clipboard == nullptr);
}

std::string Tree::currentNode()
{
	return this->curNode->value;
}

void Tree::createNode()
{
	std::string s;
	std::cout << "Введите название директории:\n";
	std::cin >> s;
	TreeNode* p = new TreeNode;
	p->value = s;
	p->level = this->curNode->level + 1;
	p->father = this->curNode;
	this->curNode->sons.push_back(p);
}

void Tree::renameNode()
{
	std::string s;
	std::cout << "Введите новое название для директории " << this->curNode->value << '\n';
	std::cin >> s;
	this->curNode->value = s;
}

void deleteSons(Tree::TreeNode* p)
{
	for (auto son : p->sons) deleteSons(son);
	delete p;
}

void Tree::deleteNode()
{
	int i{ 0 };

	if (this->curNode == this->root) return;

	for (i; i < this->curNode->father->sons.size(); i++)
	{
		if (this->curNode->father->sons.at(i) == this->curNode) break;
	}

	Tree::TreeNode* father = this->curNode->father;
	deleteSons(this->curNode);
	this->curNode = father;
	if (father != nullptr) father->sons.erase(father->sons.begin() + i);
}

void Tree::copyNode()
{
	this->clipboard = this->curNode;
}

Tree::TreeNode* copy(Tree::TreeNode* p)
{
	Tree::TreeNode* s = new Tree::TreeNode;
	s->value = p->value;
	s->level = p->level;
	s->father = p->father;
	for (auto son : p->sons)
	{
		s->sons.push_back(copy(son));
	}
	return s;
}

void Tree::pasteNode()
{
	TreeNode* p = copy(this->clipboard);
	this->curNode->sons.push_back(p);
}

void Tree::copyInFile()
{
	std::string s;
	std::cout << "Введите название файла:\n";
	std::cin >> s;
	std::ofstream out(s);
	printOut(out, this->root, 0);
}