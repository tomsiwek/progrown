#include <mpi.h>
#include <stdio.h>

double int_fun(double x)
{
    return 1/(1 + x*x);
}

int main(int argc, char* argv[])
{
    int i;
    short cpus = atoi(argv[1]);
    const int N = atoi(argv[2]);
    const int iter = N/cpus;
    double sub_sum = 0.0;
    const double h = 1.0/N;
    double sum = 0.0;
    int rank;
    double start, end;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Barrier(MPI_COMM_WORLD); /* IMPORTANT */
    start = MPI_Wtime();

    for(i = 0; i < iter; i++)
    {
        sub_sum += int_fun(((i + rank*iter) + 0.5)/N)*h;
    }

    /*printf("subtotal: %.16f\n", sub_sum);*/

    MPI_Reduce(&sub_sum, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if(rank == 0)
    {
        printf("pi=%.16f\n", 4.0*sum);
    }

    MPI_Barrier(MPI_COMM_WORLD); /* IMPORTANT */
    end = MPI_Wtime();

    MPI_Finalize();

    if (rank == 0) { /* use time on master node */
        printf("No of cpus: %d\n", cpus);
        printf("Runtime = %.8f\n", end-start);
    }

    return 0;
}
