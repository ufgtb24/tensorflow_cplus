// test2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
using namespace std;

int main()
{
	int* a = new int[3]{ 1,2,3 };
	for (int i = 0; i < 3; i++)
		cout << a[i];
    return 0;
}

