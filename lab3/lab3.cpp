/*
1.	26. ����������  �  ������  ��  �������  ������   ����������
	��������  �  �������  ������.  ���������� ���������� ���������
	��������:
		1) �������� ������ � ������ �� �����;
		2) ����� ������ ����� �  ������  �������  (��������� �����,
		������ �� ������� � �. �.); 
		3) �������������  ������ ��� �������� ����� ����� � ������,
		�� ��������������, �����������, �������� � ��������. 
		4) ���������� ������ � ����� (13).
2.	�������� ����������, ��-21
3.	Visual Studio 2022
4.
*/

#include <iostream>
#include <fstream>
#include <Windows.h>
#include "Tree.h"

int main(int argc, char* argv[])
{
	SetConsoleOutputCP(1251);
	std::ifstream in(argv[1]);
	std::ofstream out(argv[2]);
	std::string str;
	Tree t{in}; 

	int answer;
	
	while (true)
	{
		system("cls");
		(t.inRoot())
			? std::cout << "�� ���������� � �������� ���������� " << t.currentNode() << "\n�������:\n"
			: std::cout << "�� ���������� � ���������� " << t.currentNode() << "\n�������:\n0. ��������� �����\n";
		
		int sonsCount = t.printSons();

		std::cout << sonsCount << ". ������� ����������\n";
		std::cout << sonsCount + 1 << ". ������������� ����������\n";
		std::cout << sonsCount + 2 << ". ����������� ����������\n";
		std::cout << sonsCount + 3 << ". ������� ����������\n";
		std::cout << sonsCount + 4 << ". �����\n";
		if (!t.emptyClipboard()) std::cout << sonsCount + 5 << ". ��������\n";

		std::cin >> answer;

		if (answer == 0 && !t.inRoot()) t.getUp();
		else if (answer < sonsCount) t.getDown(answer);
		else if (answer == sonsCount) t.createNode();
		else if (answer == sonsCount + 1) t.renameNode();
		else if (answer == sonsCount + 2) t.copyNode();
		else if (answer == sonsCount + 3) t.deleteNode();
		else if (answer == sonsCount + 4) break;
		else if (answer == sonsCount + 5 && !t.emptyClipboard()) t.pasteNode();
	}
}