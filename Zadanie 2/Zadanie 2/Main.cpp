#define _USE_MATH_DEFINES
#include <iostream>
#include <cstdlib>
#include <omp.h>
#include <cmath>
#include <math.h>

using namespace std;
float calculationIntegral(float x, float y) {
	return (pow(M_E, -(pow(x,2) + pow(y, 2))) / (sqrt(2 + pow(x,2) + pow(y, 2))));
}

int main()
{
	double xp = 0;
	double xk = 5;
	int n = 10000;
	double dx = ((xk - xp) / (double)n);
	double dy = ((xk - xp) / (double)n);
	double integral = 0;
	double tmp = 0;
	int i, j;
	omp_set_nested(1);
	/* raz na jakiœ czas mo¿e byæ przek³amany wynik
	integral = 0;
		#pragma omp parallel shared(n) private(i)
		{
			#pragma omp for schedule(static)
			for (i = 1; i <= n; i++) {
			
				#pragma omp parallel shared(n) private(j) 
				{
					#pragma omp for reduction(+:integral) 
					for (j = 1; j <= n; j++)
					{
						integral += (calculationIntegral(xp + (i*dx), xp + (j*dx))*dx*dy);
					}
				
				}
			}
		}
	*/
	double start = omp_get_wtime();

	#pragma omp parallel shared(n) private(i, tmp)
	{
		#pragma omp for reduction(+:integral)
		for (i = 1; i <= n; i++) {
			tmp = 0;
			#pragma omp parallel shared(n) private(j) 
			{
				#pragma omp for reduction(+:tmp) 
				for (j = 1; j <= n; j++)
				{
					tmp += (calculationIntegral(xp + (i*dx), xp + (j*dx))*dx*dy);
				}
			}

			integral += tmp;
		}
	}
	printf("Calka wynosi: %f\n", integral);
	cout << "Czas wykonywania programu pierwszego: " << (omp_get_wtime() - start) << "s" << endl;
	
	double start2 = omp_get_wtime();
	integral = 0;
	for (i = 1; i <= n; i++) {
		#pragma omp parallel shared(n) private(j) num_threads(2)
		{
			#pragma omp for reduction(+:integral) 
			for (j = 1; j <= n; j++)
			{
				integral += (calculationIntegral(xp + (i*dx), xp + (j*dx))*dx*dy);
			}
		}
	}
	
	printf("Calka wynosi: %f\n", integral);
	cout << "Czas wykonywania programu drugiego: " << (omp_get_wtime() - start2) << "s" << endl;
	system("PAUSE");
	return 0;
}