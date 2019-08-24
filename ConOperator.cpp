#include "ConOperator.h"
#include <windows.h>
#include <iostream>
#include <time.h>


using namespace std;


// �趨���ӿ�ʼ���ַ������
void grid(int x, int y, string str = "")
{
	COORD pos = {2 * x, y};
	HANDLE  hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hStdOut,pos);
	if(str != "") {
		CONSOLE_CURSOR_INFO cci;
		cci.dwSize = 1;           //������ù��Ĵ�С,ֵΪ1-100
		cci.bVisible=FALSE;       //������ù���Ƿ���ʾ
		HANDLE hOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		BOOL bResult = SetConsoleCursorInfo(hOutputHandle,&cci);//���ù����Ϣ
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
	cci.dwSize = 2;           //������ù��Ĵ�С,ֵΪ1-100
	cci.bVisible = TRUE;       //������ù���Ƿ���ʾ
	HANDLE hOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	BOOL bResult = SetConsoleCursorInfo(hOutputHandle,&cci);//���ù����Ϣ

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

// �������ã�����������ı�ǰ��ɫ�ͱ���ɫ
void color(int n_color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), n_color);
}
