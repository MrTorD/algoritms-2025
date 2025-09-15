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

template <typename T>

class Stack 
{
public:

	bool is_empty()
	{
		return this->top == nullptr;
	}

	void push(T elm)
	{
		StackNode* node = new StackNode;
		node->value = elm;
		node->next = this->top;
		top = node;
	}

	T pop()
	{
		StackNode* temp = top;
		T value = top->value;
		top = top->next;
		delete temp;
		return value;
	}

private:
	struct StackNode
	{
		T value;
		StackNode* next = nullptr;
	};

	StackNode* top = nullptr;
};

int main(int argc, char* argv[]) 
{
	std::cout << "Hi" << std::endl;
	Stack<std::string> stack;
	stack.push("Hello");
	stack.push("It's");
	stack.push("Me.");
	std::cout << stack.pop() << stack.pop() << std::endl;
	std::cout << "Hi" << std::endl;
}