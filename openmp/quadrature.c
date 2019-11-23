#include <omp.h>
#include <stdio.h>
#include <math.h>

int main()
{
	int n, i;
	double h, q, x;
	int nt, id;
	double pi = M_PI;
	double t0, t1;

	n = 400000000;
	h = 1.0/n;
	q = 0.0;

	#ifdef _OPENMP /* Macro _OPENMP is set if code is compiled using OpenMP */
		t0 = omp_get_wtime();
	#endif

	#pragma omp parallel for default(none) private(x) \
	shared(n, h) reduction(+: q) schedule(static, 4) /* Pragma for parallising the for loop */
	for(i = 0; i < n; i++)
	{
		x = h*(i + 0.5);
		q += h/(1.0 + x*x);
	}

	#ifdef _OPENMP
		t1 = omp_get_wtime();
	#endif

	q = 4.0*q;
	printf("Approximated value: q = %.16f\n", q);
	printf("Error: %.16f\n", fabs(pi - q));
	
	#ifdef _OPENMP
		printf("Time for loop: %.16f\n", t1 - t0);
	#endif

	/*#pragma omp parallel default(none) shared(nt) private(id)
	{
		#pragma omp single
		nt = omp_get_num_threads();
		id = omp_get_thread_num();
		printf("Hello, my id: %2d, %2d threads total.\n", id, nt);
	}*/
	return 0;
}
