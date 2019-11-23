#include <mpi.h>
#include <stdio.h>

double int_fun(double x)
{
    return 4.0/(1.0 + x*x);
}

int main(int argc, char* argv[])
{
    int i, j;
    short cpus = atoi(argv[1]);
    const int N = atoi(argv[2]);
    const int iter = N/cpus;
    double sub_sum = 0.0;
    const double h = 1.0/N;
    double sum = 0.0; 
    int rank;
    double start;
    int tag = 66;
    MPI_Request req;
    MPI_Status stat;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    //MPI_Barrier(MPI_COMM_WORLD);
    //if(rank == 0) start = MPI_Wtime();

    for(i = 0; i < iter; i++)
    {
        sub_sum += int_fun(((i + rank*iter) + 0.5)/N)*h;
    }
    
    if(rank == 0)
    {
        sum += sub_sum;
        for(j = 1; j < cpus; j++)
        {
            MPI_Irecv(&sub_sum, 1, MPI_DOUBLE, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &req);   
            MPI_Wait(&req, &stat);
            sum += sub_sum;
            printf("Received sub-sum from process %d: %f\n", stat.MPI_SOURCE, sub_sum);
        }
        //MPI_Barrier(MPI_COMM_WORLD);
        printf("pi=%.16f\n", sum);
        printf("No of cpus: %d\n", cpus);
        //printf("Runtime = %.8f s\n", MPI_Wtime()-start);
    }
    else
    {
        /*MPI_Send(&sub_sum, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);*/
        MPI_Isend(&sub_sum, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &req);
        MPI_Wait(&req, MPI_STATUS_IGNORE);
        /*MPI_Barrier(MPI_COMM_WORLD);*/
    }
    
    MPI_Finalize();

    return 0;
}
