#include <iostream>
#include <cstring>
#include <locale.h>
#include <time.h>
#include <conio.h>
#include <sstream>
#include <stdio.h>

struct Node
{
	char* data = new char[20];
	Node* left;
	Node* right;
};

int priority(char c)
{
	switch (c)
	{
	case '+':
	case '-':
		return 1;
	case '*':
	case '/':
		return 2;
	default:
		return 100;
	}
}

bool check(char matr[][30], int N)//счетчик вложенности скобок
{
	int counter = 0;
	for (int i = 0; i < N; ++i)
	{
		if (matr[i][0] == '(') counter++;
		if (matr[i][0] == ')') counter--;
	}
	return (counter == 0);
}

Node* MakeMyTree(char matr[][30], int first, int last)//создание дерева
{
	Node* MyTree = new Node;
	int counter = 0;
	if (first == last)
	{
		strcpy(MyTree->data, matr[first]);
		MyTree->left = NULL;
		MyTree->right = NULL;
		return MyTree;
	}
	int MinPtr = 100, k, ptr;
	char c;
	for (int i = first; i <= last; ++i)
	{
		c = matr[i][0];
		if (c == '(') { counter++; continue; }
		if (c == ')') { counter--; continue; }
		if (counter > 0) continue;
		ptr = priority(c);
		if (ptr <= MinPtr)
		{
			MinPtr = ptr;
			k = i;
		}
	}
	if (MinPtr == 100 && matr[first][0] == '(' && matr[last][0] == ')')
		return MakeMyTree(matr, first + 1, last - 1);
	strcpy(MyTree->data, matr[k]);
	MyTree->left = MakeMyTree(matr, first, k - 1);
	MyTree->right = MakeMyTree(matr, k + 1, last);
	return MyTree;
}

void Term(const char* str, char matr[][30], int &N)//разбиение на термы
{
	int i = 0;
	int j = 0;
	int k = 1;
	while (str[i] != '\0')
	{
		switch (str[i])
		{
		case '+':
		case '-':
		case '*':
		case '/':
		case '(':
		case ')':
		{
			strncpy(matr[j], str + i, 1);
			matr[j][1] = '\0';
			++j;
			++i;
			break;
		}
		default:
		{
			++i;
			while ((strchr("1234567890,", str[i]) != NULL) && (str[i] != '\0'))
			{
				++i;
				++k;
			}
			strncpy(matr[j], str + i - k, k);
			matr[j][k] = '\0';
			k = 1;
			++j;
			break;
		}
		}
	}
	N = j;//количество термов
}


void PreOrder_BinaryMyTree(Node *MyTree) {
	if (MyTree)
	{

		std::cout << MyTree->data << " ";
		PreOrder_BinaryMyTree(MyTree->left);
		PreOrder_BinaryMyTree(MyTree->right);
	}
}

void SymmetricOrder_BinaryMyTree(Node *MyTree)
{
	if (MyTree)
	{
		SymmetricOrder_BinaryMyTree(MyTree->left);
		std::cout << MyTree->data << " ";
		SymmetricOrder_BinaryMyTree(MyTree->right);
	}
}


void PostOrder_BinaryMyTree(Node *MyTree)
{
	if (MyTree)
	{
		PostOrder_BinaryMyTree(MyTree->left);
		PostOrder_BinaryMyTree(MyTree->right);
		std::cout << MyTree->data << " ";
	}
}

void print(Node* MyTree)
{
	std::cout << "\nPreOrder: ";
	PreOrder_BinaryMyTree(MyTree);
	std::cout << "\nSymmetricOrder: ";
	SymmetricOrder_BinaryMyTree(MyTree);
	std::cout << "\nPostOrder: ";
	PostOrder_BinaryMyTree(MyTree);
	std::cout << '\n';
}

void unary(char matr[][30], int &N, char* str)
{
	int n;
	n = strlen(str);
	if (str[0] == '-')
	{
		for (int i = n; i > 0; i--)
		{
			str[i + 1] = str[i];
		}
		str[0] = '0';
		str[1] = '-';
		str[n + 1] = '\n';
		n = 1;
	}
	int i = 0;
	while (i < n)
	{
		if ((str[i] == '(') && (str[i + 1] == '-'))
		{
			for (int j = n + 1; j > i + 1; j--)
			{
				str[j + 1] = str[j];
			}
			str[i + 1] = '0';
			str[i + 2] = '-';
			str[n + 2] = '\0';
			n++;
		}
		else i++;
	}
}


bool IsNumber(Node *MyTree)
{
	int i = 0;
	if (!MyTree) return 0;
	while (MyTree->data[i] != '\0')
	{
		if (MyTree->data[i] == '-')
		{
			i++;
			if (strchr("0123456789,-", MyTree->data[i]) == NULL) return 0;
			++i;
		}
		else
		{
			if (strchr("0123456789,-", MyTree->data[i]) == NULL) return 0;
			++i;
		}
	}
	return 1;
}

void calculate(Node *MyTree, bool& Errorflag)
{
	float a, b, c = 0;
	if (!MyTree || !IsNumber(MyTree->left) || !IsNumber(MyTree->right)) return;
	a = atof(MyTree->left->data);
	b = atof(MyTree->right->data);
	switch (MyTree->data[0])
	{
	case '+': c = a + b; break;
	case '-': c = a - b; break;
	case '*': c = a * b; break;
	case '/':
	{
		if (b != 0)
			c = a / b;
		else Errorflag = true;
		break;
	}
	}
	delete MyTree->left;
	delete MyTree->right;
	MyTree->left = NULL;
	MyTree->right = NULL;
	sprintf(MyTree->data, "%f", c);
}

void calculateMyTree(Node *MyTree, bool& Errorflag)
{
	if (!IsNumber(MyTree))
	{
		calculateMyTree(MyTree->left, Errorflag);
		calculateMyTree(MyTree->right, Errorflag);
	}
	calculate(MyTree, Errorflag);
}

void Var(char matr[][30], char var[10], int N)
{
	int j = 0;
	for (int i = 0; i < N; ++i)
	{
		if ((strchr("+-/*0123456789(),", matr[i][0]) == NULL) && (strchr(var, matr[i][0]) == NULL))
		{
			var[j] = matr[i][0];
			j++;
		}
	}
	var[j] = '\0';
}

void Menu2(char *var, char val[][10])
{
	int i = 0;
	if (var[i] != '\0')
	{
		int h = 0;
		std::cout << "=================================" << '\n';
		std::cout << "1.Input variables" << '\n';
		std::cout << "2.Filling with random numbers" << '\n';
		std::cout << "=================================\n";
		std::cin >> h;
		switch (h)
		{
		case 1:
		{
			std::cout << "Input variables: " << '\n';
			while (var[i] != '\0')
			{
				std::cout << var[i] << " = ";
				std::cin >> val[i];
				i++;
			}
			break;
		}
		case 2:
		{
			srand(unsigned(time(NULL)));
			while (var[i] != '\0')
			{
				_itoa(rand() % 100, val[i], 10);
				std::cout << var[i] << " = " << val[i] << '\n';
				i++;
			}
			break;
		}

		}
	}
}

int index(char *str, char c)
{
	int i = 0;
	while ((*str != c) && (*str != '\0'))
	{
		str++;
		i++;
	}
	return i;
}

void swap(char matr[][30], char var[10], char val[10][10], int N)
{
	for (int i = 0; i < N; ++i)
	{
		if (strchr(var, matr[i][0]))
		{
			strcpy(matr[i], val[index(var, matr[i][0])]);
		}
	}
}

void DeleteSpace(char *str)
{
	char * ptr = str;
	while ((ptr = strstr(ptr, " ")) != NULL)
	{
		strcpy(ptr, ptr + 1);
	}
	ptr = str;
	if (*str == ' ')
		strcpy(ptr, ptr + 1);
	if (str[strlen(str) - 1] == ' ')
		str[strlen(str) - 1] = '\0';
}

void Menu(char* str, Node **MyTree)
{
	int h = 1;
	Node *TreeElement = *MyTree;
	while (h != 0)
	{
		std::cout << " =================================================" << '\n';
		std::cout << "1.Perform the task  (4 / (3 + x) ) * (y / 2 + 7)" << '\n';
		std::cout << "2.Enter the expression" << '\n';
		std::cout << "0.Exit" << '\n';
		std::cout << "==================================================" << '\n';
		std::cin >> h;
		system("CLS");
		std::cout << "==================================================" << '\n';
		switch (h)
		{
		
		case 1:
		{
			char matr[20][30];
			int N;
			std::cout << '\n' << "(4/(3+x))*(y/2+7)" << '\n';
			Term("(4/(3+x))*(y/2+7)", matr, N);
			unary(matr, N, str);
			if (!check(matr, N))  std::cout << "The entered expression isn't correctly!" << '\n';
			else
			{
				char var[10];
				Var(matr, var, N);
				char val[10][10];
				Node* MyTree = MakeMyTree(matr, 0, N - 1);
				print(MyTree);
				Menu2(var, val);
				std::cout << '\n';
				swap(matr, var, val, N);
				Node* MyTree1 = MakeMyTree(matr, 0, N - 1);
				print(MyTree1);
				bool fl = false;
				calculateMyTree(MyTree1, fl);
				std::cout << '\n';
				if (fl == false)
					std::cout << "Result:" << '\n' << MyTree1->data;
				std::cout << '\n';
				system("Pause");
				system("CLS");
			}
			break;
		}
		case 2:
		{
			char* str = new char[30];
			std::cout << "Enter the expression:" << '\n';
			gets_s(str, 30);
			gets_s(str, 30);
			DeleteSpace(str);
			while (strlen(str) == 0)
			{
				std::cout << "An empty string is entered!" << '\n';
				std::cout << "Re-type the expression:" << '\n';
				gets_s(str, 30);
				DeleteSpace(str);
			}
			char matr[20][30];
			int N;
			Term(str, matr, N);
			unary(matr, N, str);
			if (!check(matr, N))  std::cout << "The entered expression isn't correctly!" << '\n';
			else
			{
				char var[10];
				Var(matr, var, N);
				char val[10][10];
				Node* MyTree = MakeMyTree(matr, 0, N - 1);
				print(MyTree);
				Menu2(var, val);
				swap(matr, var, val, N);
				Node* MyTree1 = MakeMyTree(matr, 0, N - 1);
				print(MyTree1);
				std::cout << '\n';
				bool fl = false;
				calculateMyTree(MyTree1, fl);
				std::cout << '\n';
				if (fl == false)
					std::cout << "Result:" << '\n' << MyTree1->data;
				else  std::cout << "Division by zero!" << '\n';
				std::cout << '\n';
				system("Pause");
				system("CLS");
			}
			break;
		}
		}
	}
}

int main()
{
	setlocale(LC_ALL, "rus");
	Node * MyTree = NULL;
	Node* TreeElement = NULL;
	char* str = new char[20];
	Menu(str, &MyTree);
	system("Pause");
	return 0;
}