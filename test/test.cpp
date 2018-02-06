// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include <iostream>
using namespace std;
int a = 5;
int* b = &a;
void main() {
	*b = 2;

	cout << a << endl;
}