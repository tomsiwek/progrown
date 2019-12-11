#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 1000

int main(int* argc, char** argv)
{
    double a[N][N], a_new[N][N];
    double err = 1.0;
    const double tol = 1e-2;

    int i, j;
    int iter = 0;
    const int iter_max = 3000;

    for(i = 0; i < N; i++)
    {
        a[0][i] = 20;
        a[1][i] = 20;
        for(j = 2; j < N; j++)
        {
            a[j][i] = 10;
        }
    }

    double t0 = omp_get_wtime();

    while(err > tol && iter < iter_max)
    {
        err = 0.0;
        #pragma omp parallel
        {
        #pragma omp for reduction(max:err)
            for(i = 1; i < (N - 1); i++)
            {
                for(j = 1; j < (N - 1); j++)
                {
                    a_new[i][j] = 0.25*(a[i - 1][j] + a[i + 1][j] + a[i][j - 1] + a[i][j + 1]);
                    err = fmax(err, fabs(a_new[i][j] - a[i][j]));
                }
            }
        //#pragma omp barrier
        #pragma omp for
            for(i = 1; i < (N - 1); i++)
            {
                for(j = 1; j < (N - 1); j++)
                {
                    a[i][j] = a_new[i][j];
                }
            }
        }
        /*if((iter % 100) == 0)
        {
            printf("%d %f\n", iter, err);
        }*/
        iter++;
    }

    double t1 = omp_get_wtime();

    printf("iter: %d, error: %f, time: %f [s]\n", iter, err, t1-t0);

    return 0;
}
