#include <mpi.h>
#include <omp.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    int rank;
    int thread;
    int number;
    int size;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank == 0)
    {
        int i;
        for(i = 1; i < size; i++)
        {
            MPI_Recv(&number, 1, MPI_INT, MPI_ANY_SOURCE, 100, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("number = %d, ", number);
        }
        #pragma omp parallel private(thread)
        {
            thread = omp_get_thread_num();
            printf("A race car number %d, %d ", rank, thread);
        }
    }
    else
    {
        MPI_Send(&rank, 1, MPI_INT, 0, 100, MPI_COMM_WORLD);
    }

    printf("\n");

    MPI_Finalize();
    return 0;
}
