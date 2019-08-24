#ifndef CONOPERATOR__H_GUARD
#define CONOPERATOR__H_GUARD


#include <string>

using namespace std;

void grid(int x, int y, string str);
void grid(int x, int y, long data);
void grid(int x, int y, char c);
void grid(int x, int y, int data);

void show_cursor(int x, int y);
int mrand(int max);
void sleep(int k);
void color(int n_color);

#endif
