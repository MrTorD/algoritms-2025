/*
1.	25. Программа  на ПАСКАЛЕ включает такие сочетания ключевых
слов, как  REPEAT..UNTIL, RECORD..END, CASE..END и BEGIN..END.
Некоторые части программы могут быть  закомментированы, другие
части текста могут представлять собой  константы в апострофах.
Требуется проверить правильность вложенности этих  конструкций
с  учетом  допустимости  взаимных  вложений.  В случае  ошибок
указать номер первой некорректной строки (11).
2.	Бояринов Константин, ПС-21
3.	Visual Studio 2022
4.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <Windows.h>
#include "Stack.h"

enum KeyWord
{
	Repeat,
	Until,
	Record,
	Caseof,
	Begin,
	End,
	If,
	Then,
	Else,
	CommentStart,
	CommentEnd,
	QuotationStart,
	QuotationEnd,
	SingleQuotation,
	NotAKey
};

enum State
{
	Normal,
	Error,
	InComment,
	InQuotation
};

bool is_requieres_end(KeyWord k)
{
	return (k == Record || k == Begin || k == Caseof);
}

bool is_quotated_word(std::string s)
{
	return (s[0] == '"' && s[s.size() - 1] == '"') || (s[0] == '\'' && s[s.size() - 1] == '\'' );
}

bool can_be_nest_in_empty(KeyWord k)
{
	return (k == Begin || k == Record || k == QuotationStart || k == QuotationEnd || k == CommentStart || k == CommentEnd);
}

bool skip(State s, KeyWord k)
{
	return (k == NotAKey || (s == InComment && k != CommentEnd) || (s == InQuotation && k != QuotationEnd && k != SingleQuotation));
}

KeyWord process_word(std::string s)
{
	if (s[0] == '{' && s[s.size() - 1] == '}') return NotAKey;
	else if (s[0] == '{') return CommentStart;
	else if (s[s.size() - 1] == '}') return CommentEnd;
	else if (s == "\"" || s == "'") return SingleQuotation;
	else if (is_quotated_word(s)) return NotAKey;
	else if (s[0] == '"' || s[0] == '\'') return QuotationStart;
	else if (s[s.size() - 1] == '"' || s[s.size() - 1] == '\'') return QuotationEnd;
	else if (s == "REPEAT" || s == "repeat" || s == "Repeat") return Repeat;
	else if (s == "UNTIL" || s == "until" || s == "Until") return Until;
	else if (s == "RECORD" || s == "record" || s == "Record") return Record;
	else if (s == "CASE" || s == "case" || s == "Case") return Caseof;
	else if (s == "BEGIN" || s == "begin" || s == "Begin") return Begin;
	else if (s == "END" || s == "end" || s == "End" || s == "END." || s == "END;" || s == "end." || s == "end;") return End;
	else if (s == "IF" || s == "if" || s == "If") return If;
	else if (s == "THEN" || s == "then" || s == "Then") return Then;
	else if (s == "ELSE" || s == "else" || s == "Else") return Else;

	return NotAKey;
}

State process_state(KeyWord key_w, Stack<KeyWord>& stack, State state)
{
	if (skip(state, key_w)) return state;

	if (!can_be_nest_in_empty(key_w) && stack.is_empty()) return Error;

	KeyWord prev;
	if (!can_be_nest_in_empty(key_w)) prev = stack.pop();

	switch (key_w)
	{
		case Record:
			if (!stack.is_empty()) return Error;
			stack.push(key_w);
			break;
		case Begin:
			if (!stack.is_empty())
			{
				prev = stack.pop();
				if (prev == Record || prev == If) return Error;
				stack.push(prev, key_w);
			}
			else
				stack.push(key_w);
			break;
		case Repeat:
		case Caseof:
		case If:
			if (prev == Record || prev == If) return Error;
			stack.push(prev, key_w);
			break;
		case Until:
			while (prev == Then && !stack.is_empty()) prev = stack.pop();
			if (prev != Repeat) return Error;
			break;
		case End:
			while (prev == Then && !stack.is_empty()) prev = stack.pop();
			if (!is_requieres_end(prev)) return Error;
			break;
		case Then:
			if (prev != If) return Error;
			stack.push(key_w);
			break;
		case Else:
			if (prev != Then) return Error;
		case SingleQuotation:
			return (state == InQuotation) ? Normal : InQuotation;
		case QuotationStart:
			return InQuotation;
		case CommentStart:
			return InComment;
		case QuotationEnd:
		case CommentEnd:
			return Normal;
	}
	return state;
}

State process_line(std::string line, Stack<KeyWord>& stack, State state)
{
	std::istringstream stream(line);
	std::string str;
	KeyWord key_w;

	while (stream >> str)
	{
		key_w = process_word(str);
		state = process_state(key_w, stack, state);
	}
	return state;
}

int main(int argc, char* argv[]) 
{
	SetConsoleOutputCP(1251);
	Stack<KeyWord> stack{};

	std::ifstream in(argv[1]);
	if (!in.is_open())
	{
		std::cout << "Не найден входной файл с именем \"" << argv[1] << "\"" << std::endl;
		return 1;
	}

	unsigned int line_count{ 0 };

	std::string line;
	State state{ Normal };

	while (std::getline(in, line))
	{
		line_count++;
		state = process_line(line, stack, state);
		if (state == Error) 
		{
			std::cout << "Найдена ошибка в строке номер: " << line_count << std::endl;
			return 1;
		}
		if (state == InQuotation) state = Normal;
	}

	(stack.is_empty()) 
		? std::cout << "Синтаксис конструкций корректен" << std::endl
		: std::cout << "Конструкция не содержит закрывающего ключевого слова" << std::endl;

	return 0;
}