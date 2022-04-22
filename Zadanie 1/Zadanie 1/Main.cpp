#define _USE_MATH_DEFINES
#include <iostream>
#include <cstdlib>
#include <omp.h>
#include <cmath>
#include <math.h>

using namespace std;

float calculationIntegral(float x, float k) {
	return (pow(M_E, -(k*x)) / (1 + k));
}

int main()
{
	float xp = 0;
	float xk = 1;
	int n = 10000;
	int maxK = 100;
	float dx = ((xk - xp) / (float)n);
	float integral;
	int k, j;
	omp_set_nested(1);
	omp_set_dynamic(1);
	double start = omp_get_wtime();
	#pragma omp parallel private(k, integral) shared(maxK)
	{
		#pragma omp for schedule(dynamic)
		for (k = 1; k <= maxK; k++)
		{
			integral = 0;
			#pragma omp parallel shared(n) private(j) 
			{
				#pragma omp for reduction(+:integral) 
				for (j = 1; j <= n; j++)
				{
					integral += (calculationIntegral(xp + (j*dx), k)*dx);
				}
			}
			printf("Dla parametru k =  %d  ca\210ka wynosi: %f\n", k, integral);
		}
	}
	cout << "Czas wykonywania programu pierwszego: " << (omp_get_wtime() - start) << "s" << endl;

	double start2 = omp_get_wtime();
	for (k = 1; k <= maxK; k++)
	{
		integral = 0;
		#pragma omp parallel shared(n, maxK) private(j) 
		{
			#pragma omp for reduction(+:integral) 
			for (j = 1; j <= n; j++)
			{
				integral += (calculationIntegral(xp + (j*dx), k)*dx);
			}
		}
			printf("Dla parametru k =  %d  ca\210ka wynosi: %f\n", k, integral);
	}
	
	cout << "Czas wykonywania programu drugiego: " << (omp_get_wtime() - start2) << "s" << endl;
	system("PAUSE");
	return 0;
}