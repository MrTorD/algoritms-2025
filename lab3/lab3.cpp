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
#include <string>
#include <conio.h>

int main(int argc, char* argv[])
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	std::ifstream in(argv[1]);
	Tree t{in}; 

	std::string answer;
	
	while (!_kbhit())
	{
		system("cls");
		t.printScreen();

		if(t.handleButton(_getch()))
			return 0;
	}
}