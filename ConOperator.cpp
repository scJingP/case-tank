#include "ConOperator.h"
#include <windows.h>
#include <iostream>
#include <time.h>


using namespace std;


// 设定格子开始的字符串输出
void grid(int x, int y, string str = "")
{
	COORD pos = {2 * x, y};
	HANDLE  hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hStdOut,pos);
	if(str != "") {
		CONSOLE_CURSOR_INFO cci;
		cci.dwSize = 1;           //这个设置光标的大小,值为1-100
		cci.bVisible=FALSE;       //这个设置光标是否显示
		HANDLE hOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		BOOL bResult = SetConsoleCursorInfo(hOutputHandle,&cci);//设置光标信息
		cout << str << '\r';

		//WriteConsole(hStdOut, str.c_str(), 2, NULL, NULL);
	}
}

void grid(int x, int y, long data)
{
	COORD pos = {2 * x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
	cout << data;
}

void grid(int x, int y, int data)
{
	COORD pos = {2 * x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
	cout << data;
}

void grid(int x, int y, char c)
{
	COORD pos = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
	cout << c;
}


void show_cursor(int x, int y)
{
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 2;           //这个设置光标的大小,值为1-100
	cci.bVisible = TRUE;       //这个设置光标是否显示
	HANDLE hOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	BOOL bResult = SetConsoleCursorInfo(hOutputHandle,&cci);//设置光标信息

	COORD pos = {2 * x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}


int mrand(int max)
{
	srand((unsigned)time(0));

	return rand() % max;
}

void sleep(int k)
{
	clock_t now = clock();
	while ((clock() - now) < k);
}

// 函数作用：设置输出的文本前景色和背景色
void color(int n_color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), n_color);
}
