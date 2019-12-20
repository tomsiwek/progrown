#include <mpi.h>
#include <omp.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    int rank;
    int thread;
    int number;
    int size;
    int prov;

    MPI_Init_thread(NULL, NULL, MPI_THREAD_MULTIPLE, &prov);
    if(prov != MPI_THREAD_MULTIPLE) return 73;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank == 0)
    #pragma omp parallel private(thread)
    {
        int i;
        #pragma omp single
        for(i = 1; i < size; i++)
        {
            MPI_Recv(&number, 1, MPI_INT, MPI_ANY_SOURCE, 100 + thread, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            //printf("number = %d, ", number);
        }
        {
            thread = omp_get_thread_num();
            printf("A race car number %d, %d ", number, thread);
        }
    }
    else
    {
        #pragma omp parallel private(thread)
        MPI_Send(&rank, 1, MPI_INT, 0, 100 + thread, MPI_COMM_WORLD);
    }

    printf("\n");

    MPI_Finalize();
    return 0;
}
