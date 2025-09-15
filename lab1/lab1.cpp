/*
 1.     22. В текстовом  файле  имеется  некоторое   информационнное
письмо. Требуется сформировать ответ на письмо. Если  последнее
предложение письма является  вопросом,  начинающимся  со  слова
"кто", то слово "кто"  заменяется  словами  "конь в пальто",  а
знак вопроса заменяется на знак восклицания. В противном случае
нужно дать ответ: "Спасибо за информацию" (8).
2.  Бояринов Константин, ПС-21
3.  Visual Studio 2022
4.
 */

#include <iostream>
#include <fstream>

enum States
{
    Error = -1,
    FindQuestion = 0,
    CheckSpaceAfterWord = 1,
    FindFirstLetter = 2,
    FindSecondLetter = 3,
    FindThirdLetter = 4,
    CheckSpaceBeforeWord = 5,
    Success = 6
};

bool is_punct(char c)
{
    return (c == '?' || c == '!' || c == '.');
}

void copy_part(std::ifstream& in, std::ostream& out, long long& end)
{
    char c;
    long long i{ in.tellg() };
    for (i; i < end; i++)
    {
        c = in.get();
        out << c;
        if (c == '\n') i++;
    }
}

void copy_file(std::ifstream& in, std::ostream& out, long long& q_index, long long& w_index, long long& length)
{
    in.seekg(0, std::ios::beg);

    copy_part(in, out, w_index);
    in.seekg(3, std::ios::cur);
    out << "Конь в пальто";

    copy_part(in, out, q_index);
    out << "!";
    in.seekg(2, std::ios::cur);

    copy_part(in, out, length);
}

int process_char(char c, int state, std::ifstream& input, long long& q_index, long long& w_index)
{
    switch (state)
    {
    case FindQuestion:
        if (c == '?') q_index = -1 + input.tellg();
        return (c == '?')
            ? CheckSpaceAfterWord
            : (c == ' ' || c == '\n')
                ? FindQuestion
                : Error;
    case CheckSpaceAfterWord:
        return (c == ' ' || c == '\n' || c == ',')
            ? FindFirstLetter
            : (is_punct(c))
                ? Error
                : CheckSpaceAfterWord;
    case FindFirstLetter:
        return (c == 'О' || c == 'о')
            ? FindSecondLetter
            : (is_punct(c))
                ? Error
                : (c == ' ' || c == '\n')
                    ? FindFirstLetter
                    : CheckSpaceAfterWord;
    case FindSecondLetter:
        return (c == 'Т' || c == 'т')
            ? FindThirdLetter
            : (is_punct(c))
                ? Error
                : (c == ' ' || c == '\n')
                    ? FindFirstLetter
                    : CheckSpaceAfterWord;
    case FindThirdLetter:
        if (c == 'К' || c == 'к') w_index = -1 + input.tellg();
        return (c == 'К' || c == 'к')
            ? CheckSpaceBeforeWord
            : (is_punct(c))
                ? Error
                : (c == ' ' || c == '\n')
                    ? FindFirstLetter
                    : CheckSpaceAfterWord;
    case CheckSpaceBeforeWord:
        return (is_punct(c))
            ? Success
            : (c == ' ' || c == '\n')
                ? CheckSpaceBeforeWord
                : (c == ',')
                    ? FindFirstLetter
                    : CheckSpaceAfterWord;
    }
}

int main(int argc, char* argv[]) 
{
    std::string inputName{ argv[1] };
    std::string outputName{ argv[2] };

    std::ifstream input(inputName);
    std::ofstream output(outputName);

    if (input.is_open() && output.is_open()) {

        char c;

        input.seekg(0, std::ios::end);
        int state{ FindQuestion };
        long long length = 1 + input.tellg();

        long long word_index;
        long long question_mark_index;

        for (int i{ 1 }; i < length; i++)
        {
            input.seekg(-i, std::ios::end);

            c = input.get();

            state = process_char(c, state, input, question_mark_index, word_index);

            if (state == Error || state == Success) break;
        }

        if (state == Success || state == CheckSpaceBeforeWord)
        {
            copy_file(input, output, question_mark_index, word_index, length);
        }
        else
        {
            output << "Спасибо за информацию" << std::endl;
        }

        input.close();
        output.close();
    }
    else
    {
        std::cout << "Opening files error occured";
        return 1;
    }
    return 0;
}