#include <iostream>
#include <Windows.h>
#include <vector>
#include <queue>
#include <string>
#include <fstream>
#include <conio.h>
#include "BTree.h"

#include <algorithm>
#include <numeric>
#include <random>
#include <chrono>

const int NUMBERS_COUNT = 5000000;
const std::string NUMBERS_FILE_NAME = "5_numbers.txt";

void generateShuffled(int count, std::string outName) {
    std::vector<int> numbers(count);
    std::iota(numbers.begin(), numbers.end(), 1);

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 rng(seed);

    std::shuffle(numbers.begin(), numbers.end(), rng);

    std::ofstream outFile(outName);
    if (outFile.is_open()) {
        for (int num : numbers) {
            outFile << num << " "; 
        }
        outFile.close();
    }
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::cout << "Генерация чисел...";
    generateShuffled(NUMBERS_COUNT, NUMBERS_FILE_NAME);
    system("cls");

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

        std::cout << "1. Вывести Б-дерево в консоль\n";
        std::cout << "2. Вывести Б-дерево в файл\n";
        std::cout << "3. Добавить ключ\n";
        std::cout << "4. Найти ключ\n";
        std::cout << "5. Выход\n";

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
        {
            system("cls");
            std::cout << "Введите название файла и нажмите Enter\n";
            std::string outName;
            std::cin >> outName;
            std::ofstream out(outName);
            tree.print(out);
            out.close();
            break;
        }
        case '3':
            system("cls");
            std::cout << "Введите число и нажмите Enter\n";
            std::cin >> num;
            tree.insert(num);            
            std::cout << "Нажмите любую клавишу для продолжения...";
            _getch();
            break;
        case '4':
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
        case '5':
            return 0; 
        }
    }
}
