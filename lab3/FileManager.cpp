#include "FileManager.h"

#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <conio.h>

FileManager::FileManager(std::istream& in)
{
	std::string s;
	char c;

	int levelCounter{ 0 };
	int prevLevel{ -1 };
	bool isFile{ false };

	FileManager::Node* prevPointer = nullptr; 

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

		FileManager::Node* p = new Node;
		p->value = s;
		p->level = levelCounter;
		p->isFile = isFile;

		if (prevLevel == -1)
		{
			p->father = nullptr;
			root = prevPointer = curNode = p;
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

int FileManager::PrintSons()
{
	int i{ 0 };
	for (i; i < curNode->sons.size(); i++)
	{
		std::cout << i + 1 << ". Перейти к " << curNode->sons.at(i)->value << '\n';
	}
	return i + 1;
}

void FileManager::PrintOut(std::ostream& out, FileManager::Node* p, int level)
{
	for (int i = 0; i < level; i++)
	{
		out << '.';
	}
	out << p->value << '\n';
	for (int i = 0; i < p->sons.size(); i++)
	{
		FileManager::PrintOut(out, p->sons[i], p->sons[i]->level);
	}
}

bool FileManager::GetUp()
{
	if (curNode != root)
	{
		curNode = curNode->father;
		return true;
	}
	return false;
}

bool FileManager::GetDown(int i)
{
	if (curNode->sons.size() >= i) 
	{
		curNode = curNode->sons.at(i - 1);
		return true;
	}
	return false;
}

bool FileManager::IsRoot()
{
	return curNode == root;
}

bool FileManager::EmptyClipboard()
{
	return clipboard.p == nullptr;
}

std::string FileManager::CurrentNode()
{
	return curNode->value;
}

void FileManager::CreateNode(bool isFile)
{
	std::string s;
	(isFile) ? std::cout << "Введите название файла\n" : std::cout << "Введите название папки\n";
	std::cin >> s;
	Node* p = new Node;
	p->value = s;
	p->level = curNode->level + 1;
	p->father = curNode;
	p->isFile = isFile;
	curNode->sons.push_back(p);
}

void FileManager::RenameNode()
{
	std::string s;
	std::cout << "Введите новое название для " << curNode->value << '\n';
	std::cin >> s;
	curNode->value = s;
}

void FileManager::DeleteSons(FileManager::Node* p)
{
	for (auto son : p->sons) DeleteSons(son);
	delete p;
}

void FileManager::DeleteNode(FileManager::Node* p, bool isCutting)
{
	if (p == root) return;

	int i{ 0 };
	for (i; i < p->father->sons.size(); i++)
	{
		if (p->father->sons.at(i) == p) break;
	}

	FileManager::Node* father = p->father;

	DeleteSons(p);

	if (!isCutting) curNode = father;
	if (father != nullptr) father->sons.erase(father->sons.begin() + i);
}

void FileManager::CopyNode(bool isCutting)
{
	if (curNode == root)
		return;

	if (isCutting)
	{
		clipboard.p = curNode;
		clipboard.isCutting = true;
	}
	else 
	{
		if (clipboard.p != nullptr)
			DeleteSons(clipboard.p);

		clipboard.p = Copy(curNode, nullptr);
		clipboard.isCutting = false;
	}
}

FileManager::Node* FileManager::Copy(FileManager::Node* p, FileManager::Node* father)
{
	Node* s = new Node;
	s->value = p->value;
	(father != nullptr)
		? s->level = father->level + 1
		: s->level = 0;
	s->isFile = p->isFile;
	s->father = father;
	for (auto son : p->sons)
	{
		s->sons.push_back(this->Copy(son, s));
	}
	return s;
}

bool FileManager::CheckForNesting(Node* p, Node* potentialFather)
{
	while (p != nullptr)
	{
		if (p == potentialFather) 
			return true;
		p = p->father;
	}
	return false;
}

void FileManager::PasteNode()
{
	if (clipboard.p == nullptr || (clipboard.isCutting && CheckForNesting(curNode, clipboard.p)))
		return;

	Node* p = Copy(clipboard.p, curNode);
	curNode->sons.push_back(p);
	if (clipboard.isCutting)
	{
		DeleteNode(clipboard.p, true);
		clipboard.isCutting = false;
		clipboard.p = nullptr;
	}
}

void FileManager::PrintInFile()
{
	std::string s;
	std::cout << "Введите название файла:\n";
	std::cin >> s;
	std::ofstream out(s);
	PrintOut(out, root, 0);
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

bool FileManager::HandleButton(int ch)
{
	switch (tolower(ch))
	{
	case esc:
		return true;
	case 'r':
		RenameNode();
		break;
	case 'c':
		CopyNode(false);
		break;
	case 'x':
		CopyNode(true);
		break;
	case 'v':
	case 'м':
		PasteNode();
		break;
	case 'd':
		DeleteNode(curNode, false);
		break;
	case 'n':
		CreateNode(false);
		break;
	case 'f':
		CreateNode(true);
		break;
	case 't':
		PrintInFile();
		break;
	case arrow:
		switch (_getch())
		{
		case up:
			if (curSonIndex > 0)
				curSonIndex--;
			break;
		case down:
			if (curNode->sons.size() - 1 > curSonIndex)
				curSonIndex++;
			break;
		case left:
			if (GetUp())
				curSonIndex = 0;
			break;
		case right:
			if (GetDown(curSonIndex + 1))
				curSonIndex = 0;
			break;
		}
	break;
	}
	return false;
}