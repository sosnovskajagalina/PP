#include "pch.h"
#include <iostream>
#include <fstream>
#include <thread>
#include "time.h"
#include <chrono>
#include <vector>
using namespace std;

ifstream infile("input.txt");

int i, j, k, n;
float a[100][200], t;
int m_size;
void readMatrix()
{
	if (!infile.is_open()) 
	{
		cout << "can't open the file" << endl;
		exit(1);
	}
	if(infile)
	{
		infile >> m_size;

		for (i = 0; i < m_size; i++)
			for (j = 0; j < m_size; j++)
				infile >> a[i][j];
	}
	else
		cout << "file not found" << endl;
}

void zeroMatrix() {
	//making another matrix for example 100 010 001
	for (i = 0; i < m_size; i++)
	{
		for (j = m_size; j < 2 * m_size; j++)
		{
			if (i == j - m_size)
				a[i][j] = 1;
			else
				a[i][j] = 0;
		}
	}
}

void inverseMatrix()
{
	for (i = 0; i < m_size; i++)
	{
		t = a[i][i];
		for (j = i; j < 2 * m_size; j++)
			a[i][j] = a[i][j] / t;
		for (j = 0; j < m_size; j++)
		{
			if (i != j)
			{
				t = a[j][i];
				for (k = 0; k < 2 * m_size; k++)
					a[j][k] = a[j][k] - t * a[i][k];
			}
		}
	}
}

void printInverseMatrix()
{
	cout << "Inverse matrix\n";
	for (i = 0; i < m_size; i++)
	{
		for (j = m_size; j < 2 * m_size; j++)
			cout << a[i][j] << "\t";

		cout << "\n";
	}
	cout << "\n";
}

int main()
{
	int as = clock();

	readMatrix();
	 
	zeroMatrix();

	inverseMatrix();

	printInverseMatrix();
	
	cout << clock() - as << " ms" << endl;
	return 0;
}
