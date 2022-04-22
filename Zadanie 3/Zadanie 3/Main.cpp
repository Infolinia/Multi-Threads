#define _USE_MATH_DEFINES
#include <iostream>
#include <cstdlib>
#include <omp.h>
#include <cmath>
#include <math.h>
#include <random>
#include <ctime>

using namespace std;
int const firstMatrixVerse = 600;
int const firstMatrixColumn = 600;

int const secondMatrixVerse = 600;
int const secondMatrixColumn = 600; 

int firstMatrix[firstMatrixVerse][firstMatrixColumn];
int secondMatrix[secondMatrixVerse][secondMatrixColumn];

int resultMatrix[firstMatrixVerse][secondMatrixColumn];
int goodMatrix[firstMatrixVerse][secondMatrixColumn];


bool isShowMatrix() {
	if (firstMatrixVerse < 10 && firstMatrixColumn < 10 && secondMatrixVerse < 10 && secondMatrixColumn < 10)
		return true;
	return false;
}

void printFirstMatrix() {
	printf("\n");
	for (int i = 0; i < firstMatrixVerse; i++) {
		for (int j = 0; j < firstMatrixColumn; j++)
			printf("%d ", firstMatrix[i][j]);
		printf("\n");
	}
}

void printSecondMatrix() {
	printf("\n");
	for (int i = 0; i < secondMatrixVerse; i++) {
		for (int j = 0; j < secondMatrixColumn; j++)
			printf("%d ", secondMatrix[i][j]);
		printf("\n");
	}
}


void printGoodMatrix() {
	printf("\n");
	for (int i = 0; i < firstMatrixVerse; i++) {
		for (int j = 0; j < secondMatrixColumn; j++)
			printf("%d ", goodMatrix[i][j]);
		printf("\n");
	}
}

void printResultMatrix() {
	printf("\n");
	for (int i = 0; i < firstMatrixVerse; i++) {
		for (int j = 0; j < secondMatrixColumn; j++)
			printf("%d ", resultMatrix[i][j]);
		printf("\n");
	}
}

void checkResultMatrix() {
	for (int i = 0; i < firstMatrixVerse; i++) {
		for (int j = 0; j < secondMatrixColumn; j++) {
			if (goodMatrix[i][j] != resultMatrix[i][j]) {
				printf("Wykryto zla wartosc, powinno byæ: %d, a jest: %d\n", goodMatrix[i][j], resultMatrix[i][j]);
			}
		}
	}
}


int main()
{
	if (firstMatrixColumn == secondMatrixVerse) {
		srand(time(NULL));
		random_device rd;
		mt19937 mt(rd());
		uniform_int_distribution<int> dist(1, 50);
		int tmp = 0;
		omp_set_nested(1);
		for (int i = 0; i < firstMatrixVerse; i++) {
			for (int j = 0; j < firstMatrixColumn; j++)
				firstMatrix[i][j] = dist(mt);
		}

		if (isShowMatrix())
			printFirstMatrix();
		
		for (int i = 0; i < secondMatrixVerse; i++) {
			for (int j = 0; j < secondMatrixColumn; j++)
				secondMatrix[i][j] = dist(mt);
		}

		if(isShowMatrix())
			printSecondMatrix();
		
		printf("\nLicze bez zrownoleglenia i zapisuje wyniki.\n");
		double start1 = omp_get_wtime();
		for (int m = 0; m < firstMatrixVerse; m++)
		{
			for (int n = 0; n < secondMatrixColumn; n++)
			{
				int c = 0;
				for (int o = 0; o < secondMatrixVerse; o++)
				{
					c += (firstMatrix[m][o] * secondMatrix[o][n]);
				}
				goodMatrix[m][n] = c;
			}
		}

		cout << "Czas wykonywania programu bez zrownoleglenia: " << (omp_get_wtime() - start1) << "s" << endl;
		if (isShowMatrix())
			printGoodMatrix();

		printf("Licze z wykorzystaniem OMP i porownuje wyniki - pierwsza metoda\n");
		double start2 = omp_get_wtime();

		#pragma omp parallel for
		for (int m = 0; m < firstMatrixVerse; m++)
		{
			#pragma omp parallel for private(tmp)
			for (int n = 0; n < secondMatrixColumn; n++)
			{
				tmp = 0;
				for (int o = 0; o < secondMatrixVerse; o++)
				{
					tmp += (firstMatrix[m][o] * secondMatrix[o][n]);
				}
				resultMatrix[m][n] = tmp;
			}
		}
		checkResultMatrix();
		cout << "Czas wykonywania programu pierwszego: " << (omp_get_wtime() - start2) << "s" << endl;

		printf("Licze z wykorzystaniem OMP i porownuje wyniki - druga metoda\n");
		double start3 = omp_get_wtime();

		for (int m = 0; m < firstMatrixVerse; m++)
		{
			for (int n = 0; n < secondMatrixColumn; n++)
			{
				tmp = 0;
				#pragma omp parallel shared(secondMatrixVerse)
				{
					#pragma omp for reduction(+:tmp) 
					for (int o = 0; o < secondMatrixVerse; o++)
					{
						tmp += (firstMatrix[m][o] * secondMatrix[o][n]);
					}
					resultMatrix[m][n] = tmp;
				}
			}
		}
		checkResultMatrix();
		cout << "Czas wykonywania programu drugiego: " << (omp_get_wtime() - start3) << "s" << endl;
	}
	system("PAUSE");
	return 0;
}