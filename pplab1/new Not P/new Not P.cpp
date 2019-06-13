#include "pch.h"
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

int threadCount = 1;

struct Argumets {
	vector<vector<int>>* origin;
	vector<vector<int>>* result;
	int start;
	int end;
	int determinant;
};

vector<vector<int>> readMatrix() {
	ifstream fin("input.txt");
	int n;
	fin >> n;
	vector<vector<int>> matrix(n, vector<int>(n));
	for (int y = 0; y < n; y++) {
		for (int x = 0; x < n; x++) {
			fin >> matrix[y][x];
		}
	}
	return matrix;
}

int findDeterminant(vector<vector<int>> matrix) {
	int res = 0;
	int mult = 1;
	if (matrix.size() < 1)
	{
		return 0;
	}
	if (matrix.size() == 1)
	{
		res = matrix[0][0];
	}
	else if (matrix.size() == 2) {
		res = matrix[0][0] * matrix[1][1] - matrix[1][0] * matrix[0][1];
	}
	else
	{
		for (int x = 0; x < matrix.size(); x++) {
			int diagonal = 1;
			for (int y = 0; y < matrix.size(); y++) {
				diagonal *= matrix[y][(x + y) % matrix.size()];
			}
			res += diagonal;

			diagonal = 1;
			for (int y = 0; y < matrix.size(); y++) {
				diagonal *= matrix[y][(matrix.size() - y + x) % matrix.size()];
			}

			res -= diagonal;
		}
	}
	return res;
}

vector<vector<int>> getMinor(vector<vector<int>>* matrixP, int row, int col) {
	auto matrix = *matrixP;
	vector<vector<int>> minor(matrix.size() - 1, vector<int>(matrix.size() - 1));
	bool isYpassed = false;
	bool isXpassed = false;

	for (int x = 0; x < matrix.size(); x++) {
		if (x == col) {
			isXpassed = true;
			continue;
		}
		for (int y = 0; y < matrix.size(); y++) {
			if (y == row) {
				isYpassed = true;
				continue;
			}
			minor[y - (isYpassed ? 1 : 0)][x - (isXpassed ? 1 : 0)] = matrix[y][x];
		}
		isYpassed = false;
	}
	return minor;
}

int findAlgebraicAddition(vector<vector<int>>* matrixP, int row, int col) {
	return findDeterminant(getMinor(matrixP, row, col)) * ((row + col) % 2 == 1 ? -1 : 1);
}

DWORD WINAPI ThreadFunc(PVOID pvParam) {
	Argumets *matrixDescriptorP = (Argumets*)pvParam;
	std::vector<std::vector<int>> matrix;
	int det = findDeterminant(matrix);
	for (int col = matrixDescriptorP->start; col < matrixDescriptorP->end; col++) {
		for (int x = 0; x < matrixDescriptorP->origin->size(); x++) {
			(*(matrixDescriptorP->result))[x][col] = findAlgebraicAddition(matrixDescriptorP->origin, col, x) / matrixDescriptorP->determinant;
		}
	}
	ExitThread(0);
}

vector<vector<int>> getInverseMatrix(vector<vector<int>> matrix) {
	vector<vector<int>> back(matrix.size(), vector<int>(matrix.size()));

	int determinant = findDeterminant(matrix);

	if (determinant < 0.05 && determinant > -0.05) {
		cout << "Determinant is equal to zero" << endl;
		return back;
	}

	if (matrix.size() < threadCount) {
		threadCount = matrix.size();
	}

	vector<int> bounds(threadCount + 1);
	int step = matrix.size() / threadCount;
	int currentStep = 0;

	for (int i = 1; i <= threadCount; i++) {
		currentStep += step;
		bounds[i] = currentStep;
	}
	bounds[bounds.size() - 1] = matrix.size();
	int indexThread = 0;
	HANDLE* handles = new HANDLE[threadCount];

	for (int row = 1; row < bounds.size(); row++) {
		// Запуск в отдельном потоке
		if (indexThread == threadCount) {
			WaitForMultipleObjects(threadCount, handles, TRUE, INFINITE);
			indexThread = 0;
		}
		Argumets* matrixDescriptor = new Argumets;
		matrixDescriptor->origin = &matrix;
		matrixDescriptor->result = &back;
		matrixDescriptor->start = bounds[row - 1];
		matrixDescriptor->end = bounds[row];
		matrixDescriptor->determinant = determinant;
		LPVOID matrixDescriptorThP = (LPVOID)matrixDescriptor;
		handles[row - 1] = CreateThread(NULL, 4096, &ThreadFunc, matrixDescriptorThP, 0, NULL);
	}

	WaitForMultipleObjects(threadCount, handles, true, INFINITE);

	return back;
}


int main(int argc, const char * argv[]) {
	if (argc < 2) {
		std::cout << "invalid arguments\n";
	}
	unsigned long start_time = clock();
	threadCount = std::stod(argv[1]);
	vector<vector<int>> matrix = readMatrix();
	vector<vector<int>> result = getInverseMatrix(matrix);

	unsigned long end_time = clock();
	unsigned long search_time = end_time - start_time;
	cout << "Done" << endl;
	cout << search_time << " ms." << endl;


	return 0;
}
