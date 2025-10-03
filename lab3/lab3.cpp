/*
1.	26. Информация  о  файлах  на  жестких  дисках   компьютера
	записана  с  помощью  дерева.  Обеспечить выполнение следующих
	операций:
		1) загрузку дерева в память из файла;
		2) обход дерева папок в  режиме  диалога  (раскрытие папок,
		подъем на уровень и т. п.); 
		3) корректировку  дерева при создании новых папок и файлов,
		их переименовании, копировании, переносе и удалении. 
		4) сохранение дерева в файле (13).
2.	Бояринов Константин, ПС-21
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