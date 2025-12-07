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

const bool GENERATE_NUMBERS = false;
const int NUMBERS_COUNT = 50000000;
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

    if (GENERATE_NUMBERS)
    {
        std::cout << "Генерация чисел...";
        generateShuffled(NUMBERS_COUNT, NUMBERS_FILE_NAME);
        system("cls");
    }

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
    std::string value;
    while (in >> key >> value)
    {
        tree.insert(key, value);
    }

    while (true)
    {
        system("cls");

        tree.printStructure(std::cout);

        std::cout << "\nВведите:\n";
        std::cout << "1. Вывысти Б-дерево в файл\n";
        std::cout << "2. Добавить ключ\n";
        std::cout << "3. Найти ключ\n";
        std::cout << "4. Сохранить в файл\n";
        std::cout << "5. Выход\n";

        int ch = _getch();

        switch (ch)
        {
        case '1':
        {
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            system("cls");
            std::cout << "Введите название файла и нажмите Enter\n";
            std::string outName;
            std::cin >> outName;
            std::ofstream out(outName);
            tree.printStructure(out);
            out.close();
            break;
        }
        case '2':
        {
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            int key;
            std::string value;
            system("cls");
            std::cout << "Введите число и нажмите Enter\n";
            std::cin >> key;
            std::cout << "Введите значение и нажмите Enter\n";
            std::cin >> value;
            tree.insert(key, value);
            std::cout << "Нажмите любую клавишу для продолжения...";
            _getch();
            break;
        }
        case '3':
        {
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            int key;
            system("cls");
            std::cout << "Введите ключ и нажмите Enter\n";
            std::cin >> key;

            if (!tree.find(key)) {
                std::cout << "Ключ не найден\n";
            }
            std::cout << "Нажмите любую клавишу для продолжения...";
            _getch();
            break;
        }
        case '4':
        {
            std::ofstream in(inName);
            tree.printValues(in);
            in.close();
            break;
        }
        case '5':
            return 0; 
        }
    }
}
