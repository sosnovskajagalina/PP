// pp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <stdio.h>
#include <conio.h>
#include <windows.h> 
#include "time.h"
#include <iostream>
using namespace std;
const int n = 3;
int a[n][n] = { 1, -2, 0, 4, 6, 2, -3, 4, -2 };
int b[n][n] = { 0, 2, 0, 1, 1, 1, 5, -3, 10 };
int c[n][n];

DWORD WINAPI ThreadFunc(PVOID pvParam)
{
	int num, i, j;
	num = (*((int *)pvParam));
	for (i = 0; i < n; i++)
	{
		c[num][i] = 0;
		for (j = 0; j < n; j++)
			c[num][i] += a[num][j] * b[j][i];
	}

	DWORD dwResult = 0;
	return dwResult;
}

int main()
{
	int i, j;
	int x[n];

	DWORD dwThreadId[n - 1], dw;
	HANDLE hThread[n - 1];

	for (i = 1; i < n; i++)
	{
		x[i] = i;
		hThread[i - 1] = CreateThread(NULL, 0, ThreadFunc, (PVOID)&x[i], 0, &dwThreadId[i - 1]);
		if (!hThread) printf("main process: thread %d not execute!", i);
	}

	x[0] = 0;
	ThreadFunc((PVOID)&x[0]);

	dw = WaitForMultipleObjects(n - 1, hThread, TRUE, INFINITE);

	for (i = 0; i < n; i++)
	{
		fprintf(stdout, "\n");
		for (j = 0; j < n; j++) printf("%d ", c[i][j]);
	}
	int t = clock();
	cout << clock() - t << " ms" << endl;
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
