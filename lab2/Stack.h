#pragma once
template <typename T>

class Stack
{
public:
	Stack()
	{
		this->top = nullptr;
	}

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

	void push(T elm1, T elm2)
	{
		this->push(elm1);
		this->push(elm2);
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
		StackNode* next = nullptr;
		T value;
	};

	StackNode* top;
};