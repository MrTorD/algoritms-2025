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
			? std::cout << "Вы находитесь в корневой директории " << t.currentNode() << "\nНажмите:\n"
			: std::cout << "Вы находитесь в директории " << t.currentNode() << "\nНажмите:\n0. Вернуться назад\n";
		
		int sonsCount = t.printSons();

		std::cout << sonsCount << ". Создать директорию\n";
		std::cout << sonsCount + 1 << ". Переименовать директорию\n";
		std::cout << sonsCount + 2 << ". Скопировать директорию\n";
		std::cout << sonsCount + 3 << ". Удалить директорию\n";
		std::cout << sonsCount + 4 << ". Выход\n";
		if (!t.emptyClipboard()) std::cout << sonsCount + 5 << ". Вставить\n";

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