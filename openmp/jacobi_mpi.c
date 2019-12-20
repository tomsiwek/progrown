#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <omp.h>
#include <mpi.h>

#define N 1000 /* liczba kolumn */
#define M 1000 /* liczba wierszy */

/* Poprawic skalowalnosc -- komunikacja! */

int main(int argc, char** argv)
{
    /*double a[N][M], a_new[N][M];*/
    double err = 1.0;
    const double tol = 1e-3;

    int i, j;
    int iter = 0;
    const int iter_max = 3000;

    int prov;
    int rank;
    int size;
    int source;
    int target;
    MPI_Status status;
    MPI_Request request;

    double* buf1, * buf2;
    double** a, ** a_new;
    double tmp_err;

    double t0, t1;

    MPI_Init_thread(NULL, NULL, MPI_THREAD_SINGLE, &prov);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int m = M/size;

    /* dynamiczna alokacja a i a_new o wymiarach m*N */
    a = (double**)malloc(m*sizeof(double*));
    a_new = (double**)malloc(m*sizeof(double*));
    for(i=0; i < m; i++)
    {
        a[i] = (double*)malloc(N*sizeof(double));
        a_new[i] = (double*)malloc(N*sizeof(double));
    }

    buf1 = (double*)malloc(N*sizeof(double));
    buf2 = (double*)malloc(N*sizeof(double));

    printf("MPI process No %d\n", rank);
    printf("Upper bound on the number of threads: %d\n", omp_get_max_threads());
    printf("Number of cpus: %d\n\n", omp_get_num_procs());

    for(i = 0; i < N; i++)
    {
        a[0][i] = 20;
        a[1][i] = 20;
        for(j = 2; j < m; j++)
        {
            a[j][i] = 10;
        }
    }

    t0 = MPI_Wtime();

    while(err > tol && iter < iter_max)
    {
        err = 0.0;

        /* buf1 staje sie ostatnim wierszem a */
        /* buf2 zostaje skopiowany jako pierwszy (0)  wiersz a */
        
        if(rank == 0)
        {
            source = MPI_PROC_NULL;
        }
        else
        {
            source = rank - 1;
        }
        if(rank == size - 1)
        {
            target = MPI_PROC_NULL;
        }
        else
        {
            target = rank + 1;
        }

        MPI_Isend(a[m - 1], N, MPI_DOUBLE, target, 1, MPI_COMM_WORLD, &request);
        MPI_Recv(buf1, N, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);

        /* podmiana wierszy a na buf2 */

        /* buf1 staje sie pierwszym (0) wierszem a */
        /* buf2 zostaje skopiowany jako ostatni  wiersz a */

        if(rank == 0)
        {
            target = MPI_PROC_NULL;
        }
        else
        {
            target = rank - 1;
        }
        if(rank == size - 1)
        {
            source = MPI_PROC_NULL;
        }
        else
        {
            source = rank + 1;
        }

        MPI_Isend(a[0], N, MPI_DOUBLE, target, 2, MPI_COMM_WORLD, &request);
        MPI_Recv(buf2, N, MPI_DOUBLE, source, 2, MPI_COMM_WORLD, &status);

        /* podmiana wierszy a na buf2 */

        if(rank != 0)
        {
            #pragma omp parallel for reduction(max:err) collapse(1)
            for(j = 1; j < (N - 1); j++)
            {
                a_new[0][j] = 0.25*(buf1[j] + a[1][j] + a[0][j - 1] + a[0][j + 1]);
                err = fmax(err, fabs(a_new[0][j] - a[0][j]));
            }
        }
        if(rank != (size - 1))
        {
            #pragma omp parallel for reduction(max:err) collapse(1)
            for(j = 1; j < (N - 1); j++)
            {
                a_new[m - 1][j] = 0.25*(a[m - 2][j] + buf2[j] + a[m - 1][j - 1] + a[m - 1][j + 1]);
                err = fmax(err, fabs(a_new[m - 1][j] - a[m - 1][j]));
            }
        }
        #pragma omp parallel for reduction(max:err) collapse(2)
        for(i = 1; i < (m - 1); i++)
        {
            for(j = 1; j < (N - 1); j++)
            {
                a_new[i][j] = 0.25*(a[i - 1][j] + a[i + 1][j] + a[i][j - 1] + a[i][j + 1]);
                err = fmax(err, fabs(a_new[i][j] - a[i][j]));
            }
        }

        if(rank != 0)
        {
            #pragma omp parallel for collapse(1) schedule(static,1)
            for(j = 1; j < (N - 1); j++)
            {
                a[0][j] = a_new[0][j];
            }
        }
        if(rank != (size - 1))
        {
            #pragma omp parallel for collapse(1) schedule(static,1)
            for(j = 1; j < (N - 1); j++)
            {
                a[m - 1][j] = a_new[m - 1][j];
            }
        }
        #pragma omp parallel for collapse(2) schedule(static,1)
        for(i = 1; i < (m - 1); i++)
        {
            for(j = 1; j < (N - 1); j++)
            {
                a[i][j] = a_new[i][j];
            }
        }

        MPI_Allreduce(&err, &tmp_err, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
        err = tmp_err;

        if((iter % 100) == 0 && rank == 0)
        {
            printf("%d %f\n", iter, err);
        }

        iter++;
    }

    t1 = MPI_Wtime();

    MPI_Finalize();
    
    if(rank == 0) printf("iter: %d, error: %f, time: %f [s]\n\n", iter, err, t1-t0);

    return 0;
}
