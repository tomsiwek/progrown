#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <mpi.h>

#define N 1000
#define M 1000

int main(int argc, char** argv)
{
    double a[N][M], a_new[N][M];
    double err = 1.0;
    const double tol = 1e-3;

    int i, j;
    int iter = 0;
    const int iter_max = 3000;

    int prov;
    int rank;
    int size;

    double t0, t1;

    MPI_Init(NULL, NULL, MPI_THREAD_SINGLE, &prov);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int m = M/size;

    /* dynamiczna alokacja a i a_new o wymiarach N*m */
    **a = malloc(N*sizeof(double*));
    for(i=0; i < m; i++)
    {
        a[i] = malloc(m*sizeof(double));
        a_new[i] = malloc(m*sizeof(double));
    }

    printf("Upper bound on the number of threads: %d\n", omp_get_max_threads());
    printf("Number of cpus: %d\n", omp_get_num_procs());

    for(i = 0; i < N; i++)
    {
        a[0][i] = 20;
        a[1][i] = 20;
        for(j = 2; j < m; j++)
        {
            a[j][i] = 10;
        }
    }

    t0 = MPI_WTIME();

    while(err > tol && iter < iter_max)
    {
        err = 0.0;

        /* buf1 staje sie ostatnim wierszem a */
        /* buf2 zostaje skopiowany jako pierwszy (0)  wiersz a */
        
        buf1 = a[n-1];

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

        MPI_Isend(buf1, N, MPI_DOUBLE, target, 1, MPI_COMMW_WORLD, &request);
        MPI_Recv(&buf2, n, MPI_DUBLE, source, 1, MPI_COMM_WORLD, &status);

        for(i = 0; i < N; i++)
        {
            a[0][i] = buf2[i];
        }

        /* podmiana wierszy a na buf2 */

        /* buf1 staje sie pierwszym (0) wierszem a */
        /* buf2 zostaje skopiowany jako ostatni  wiersz a */

        buf1 = a[0];

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

        MPI_Isend(buf1, N, MPI_DOUBLE, target, 2, MPI_COMMW_WORLD, &request);
        MPI_Recv(&buf2, n, MPI_DUBLE, source, 2, MPI_COMM_WORLD, &status);

        /* podmiana wierszy a na buf2 */

        for(i = 0; i < N; i++)
        {
            a[n-1][i] = buf2[i];
        }

        #pragma omp parallel for reduction(max:err) collapse(2)
        for(i = 1; i < N; i++)
        {
            for(j = 1; j < m; j++)
            {
                a_new[i][j] = 0.25*(a[i - 1][j] + a[i + 1][j] + a[i][j - 1] + a[i][j + 1]);
                err = fmax(err, fabs(a_new[i][j] - a[i][j]));
            }
        }
        #pragma omp parallel for collapse(2) schedule(static,1)
        for(i = 1; i < N; i++)
        {
            for(j = 1; j < m - 1; j++)
            {
                a[i][j] = a_new[i][j];
            }
        }

        MPI_Allreduce(&err, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

        if((iter % 100) == 0)
        {
            printf("%d %f\n", iter, err);
        }
        iter++;
    }

    MPI_Finalize();

    t1 = MPI_WTIME();

    printf("iter: %d, error: %f, time: %f [s]\n", iter, err, t1-t0);

    return 0;
}
