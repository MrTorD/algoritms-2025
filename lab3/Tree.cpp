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

	Tree::TreeNode* prevPointer = nullptr; 

	std::getline(in, s);
	Tree::TreeNode* p = new TreeNode;
	p->value = s;
	p->father = nullptr;
	p->level = 0;
	int prevLevel = 0;
	prevPointer = p;
	this->root = this->curNode = p;

	while (!in.eof() && in.good())
	{
		levelCounter = 0;
		do 
		{
			in.get(c);
			
			if (c == '.')
			{
				levelCounter++;
			}
			else
			{
				std::getline(in, s);
				s = c + s;
			}

		} while (c == '.');

		Tree::TreeNode* p = new TreeNode;
		p->value = s;
		p->level = levelCounter;

		if (levelCounter == prevLevel)
		{
			p->father = prevPointer->father;
			
			prevPointer->father->sons.push_back(p);
			prevPointer = p;
		} 
		else if (levelCounter < prevLevel)
		{
			do
			{
				prevPointer = prevPointer->father;
				prevLevel = prevPointer->level;
			} while (levelCounter < prevLevel);

			p->father = prevPointer->father;
			prevPointer->father->sons.push_back(p);
			prevLevel = levelCounter;
			prevPointer = p;
		}
		else
		{
			prevPointer->sons.push_back(p);
			p->father = prevPointer;
			prevPointer = p;
			prevLevel = levelCounter;
		}
	}
	prevPointer = p;
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