#include <iostream>
#include <Windows.h>
#include <vector>
#include <queue>
#include <string>
#include <fstream>
#include <conio.h>
#include "BTree.h"

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::cout << "Введите порядок дерева и название входного файла с числами\n";
    int level;
    std::string inName;
    std::cin >> level >> inName;
    std::ifstream in(inName);

    while (level < 2)
    {
        std::cout << "Указан неверный порядок дерева, введите целое число не меньше 2\n";
        std::cin >> level;
    }

    BTree tree(level);

    int key;
    while (in >> key)
    {
        tree.insert(key);
    }

    while (true)
    {
        system("cls");
        std::cout << "Введите:\n";

        std::cout << "1. Вывести Б-дерево\n";
        std::cout << "2. Добавить ключ\n";
        std::cout << "3. Найти ключ\n";
        std::cout << "4. Выход\n";

        int ch = _getch();

        int num;
        switch (ch)
        {
        case '1':
            system("cls");
            tree.print(std::cout);
            std::cout << "Нажмите любую клавишу для продолжения...";
            _getch();
            break;
        case '2':
            system("cls");
            std::cout << "Введите число и нажмите Enter\n";
            std::cin >> num;
            tree.insert(num);            
            std::cout << "Нажмите любую клавишу для продолжения...";
            _getch();
            break;
        case '3':
            system("cls");
            std::cout << "Введите число и нажмите Enter\n";
            std::cin >> num;

            if (tree.find(num)) {
                std::cout << "Ключ найден\n";
            }
            else {
                std::cout << "Ключ не найден\n";
            }
            std::cout << "Нажмите любую клавишу для продолжения...";
            _getch();
            break;
        case '4':
            return 0; 
        }
    }
}
