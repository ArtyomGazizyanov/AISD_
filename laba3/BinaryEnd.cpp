// BinaryEnd.cpp: определяет точку входа для консольного приложения.
//
/*20. Имеется  некоторое  алгебраическое выражение.  Операнды
заданы идентификаторами,  операции выполняются по приоритетам,
допускается использование круглых скобок.  Требуется построить
и выдать в  наглядном  виде  бинарное  дерево,  представляющее
данное выражение (12)*/

#include "stdafx.h"
#include<stdio.h>
#include<iostream>
#include<conio.h>
#include <stack>
#include <vector>
#include<string> 

#include "Structures.h"

using namespace std; 



char Unary_plus = '#';
char Unary_minus = '@';

int Precision(string symbol)
{
	if (symbol == "(" || symbol == ")")
		return 0;
	if (symbol == "+" || symbol == "-")
		return 1;
	if (symbol == "*" || symbol == "/")
		return 2;
	if (symbol == "^")
		return 3;
	if (symbol == "@" || symbol == "#")
		return 4;
	return 0;
}

bool isOperator(string mathOperator)
{
	return (mathOperator == ("@") || mathOperator == ("#") ||  mathOperator == "+" || mathOperator == "-" || mathOperator == "*" || mathOperator == "/" || mathOperator == "^" || mathOperator == ")" || mathOperator == "(");
}

string FindUnaryOperators(string inputString)
{	
	for (size_t i = 0; i < inputString.length(); i++)
	{
		if (inputString[0] == '-' || inputString[0] == '+')
		{
			(inputString[0] == '-') ? inputString[0] = '@' : inputString[0] = '#';
			//cout << "Replace in the begining" << endl;
		}
		else if(i != 0 && inputString[i - 1] == '(' && (inputString[i] == '-' || inputString[i] == '+'))
		{
			(inputString[i] == '-') ? inputString[i] = '@' : inputString[i] = '#';
			//cout << "Replace in (" << endl;
		}
	}
	return inputString;
}

vector <string> TransformInfixintoPostfix(string inputExpresion)
{
	string charTostring;
	stack<string> stack;	
	bool isValidExpresion = true;
	std::vector <string> postfixExpresion;
	bool isOperand = false;
	string operandName = "";
	int openFrame = 0;
	int closeFrame = 0;
	inputExpresion = FindUnaryOperators(inputExpresion);

	for (size_t i = 0; i < inputExpresion.length(); i++)
	{
		charTostring = inputExpresion[i];
		if (!isOperator(charTostring))
		{
			operandName += charTostring;
			if (i != inputExpresion.length() - 1)
			{
				i++;
				charTostring = inputExpresion[i]; 
				while (!isOperator(charTostring))
				{
					operandName += charTostring;
					i++;
					charTostring = inputExpresion[i]; 
				} 
			} 
			postfixExpresion.push_back(operandName);
			operandName = ""; 
		} 
		if (isOperator(charTostring))
		{ 
			if	(stack.size() == 0) //первый элмент стека
			{ 
				stack.push(charTostring); 
			}
			else if (inputExpresion[i] == ')' || inputExpresion[i] == '(')
			{
				if (inputExpresion[i] == '(')//открывающую скобку записываем в стек
				{ 
					openFrame++;
					stack.push(charTostring);
				}
				else  if (inputExpresion[i] == ')' && openFrame >= 0) // выбрасываем все символы из стека в output 
				{
					openFrame--;
					while (stack.top() != "(" && stack.size() != 0)
					{
						postfixExpresion.push_back(stack.top());
						stack.pop(); 
					}
					if (!stack.empty() && stack.top() == "(")
					{
						stack.pop(); 
					}
				}//(8+2*5)/(1+3*2-4)
				//-b в самом начале смотреть минус и змаенять его на маркер или сразу после открытой скобки
				//перевернуть вывод вверх дном
			}
			else
			{ 
				charTostring = inputExpresion[i]; 
				if (Precision(charTostring) < Precision(stack.top()))
				{
					while (stack.size() > 0 && (Precision(charTostring) <= Precision(stack.top()) && stack.top() != "(" ))
					{
						postfixExpresion.push_back(stack.top());
						stack.pop(); 
					}

					stack.push(charTostring);
				}
				else
				{
					stack.push(charTostring);
				}
			}
		}
	}
	while (stack.size() != 0)
	{
		//cout << "rest = " << stack.top() << endl;
		postfixExpresion.push_back(stack.top());
		stack.pop();
	}

	return postfixExpresion;
}
 

std::stack <TreeLeaf*> CreateExpresionTree(std::vector <string> inputExpression)
{
	std::stack <TreeLeaf*> treeStack;
	TreeLeaf *newNode;  
	for (int i = 0; i < inputExpression.size(); i++)
	{ //continue till end of the expr.  
		if (!isOperator(inputExpression[i]))//if symbol is operand.
		{ 
			newNode = new TreeLeaf;
			newNode->value = inputExpression[i];
			newNode->left = NULL;
			newNode->right = NULL;
			treeStack.push(newNode);//TODO:: разобраться с сылками и стэками
		}
		else
		{ //If the symbol is operator//pop two top elements. 
			newNode = new TreeLeaf;  
			newNode->value = inputExpression[i];
			newNode->left = treeStack.top(); 
			treeStack.pop();
			if (!treeStack.empty())// && 
			{
				if ((treeStack.top())->value == "@" || (treeStack.top())->value == "@")
				{
					(treeStack.top()->value == "@" ?  treeStack.top()->value = "-" : treeStack.top()->value = "+");
				}
				newNode->right = treeStack.top(); 
				if (!treeStack.empty())
				{
					treeStack.pop();
				}
			}
			else
			{
				newNode->right = NULL;
			}
			treeStack.push(newNode);
		} 
	}
	return treeStack;
}

void PrinTree(TreeLeaf* currNode, string deep)
{ 
	if (currNode != NULL)//Пока не встретится пустой узел
	{
		cout << deep + currNode->value << endl; //Отображаем корень дерева
		deep += "_|";
		PrinTree(currNode->right, deep); //Рекурсивная функция для правого поддерева
		PrinTree(currNode->left, deep); //Рекурсивная функция для левого поддерева
	} 
}

void PrintPostfixExpresion(std::vector <string> postfixExpresion)
{
	for (size_t i = 0; i < postfixExpresion.size(); ++i)
	{
		cout << postfixExpresion[i];
	}
	cout << endl;
}

void DeleteTree(TreeLeaf* currNode)
{
	if (currNode != NULL)
	{
		DeleteTree(currNode->left);
		DeleteTree(currNode->right);
		free(currNode);
	}
}


int main()
{
	string inputExpresion;
	std::vector <string> postfixExpresion;
	std::stack <TreeLeaf*> expresionTree;

	cout << "\nEnter an infix expression: ";
	std::cin >> inputExpresion;

	postfixExpresion = TransformInfixintoPostfix(inputExpresion);
	
	cout << "The Postfix expression is: ";
	PrintPostfixExpresion(postfixExpresion);

	TreeLeaf treeNode;
	expresionTree = CreateExpresionTree(postfixExpresion);
	
	TreeLeaf *topNode = expresionTree.top();
	string deep = "";
	PrinTree(topNode, deep);

	DeleteTree(topNode);
	postfixExpresion.clear();

	string pause;
	std::cin >> pause;

	return 0;
}
