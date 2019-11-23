#include <mpi.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int numranks;
    MPI_Comm_size(MPI_COMM_WORLD, &numranks);
    int src, dest;
    int tag = 42;
    MPI_Status status;

    if(rank != 0) /* procesy o niezerowej randze */
    {
        dest = 0;
        MPI_Send(&rank, 1, MPI_SHORT, dest, tag, MPI_COMM_WORLD);
    }
    else /* proces o randze zero */
    {
        short curr_rank;
        int sum = 0;
        for(src = 1; src < numranks; src++)
        {    
	    MPI_Recv(&curr_rank, 1, MPI_SHORT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
            printf("Received rank: %d\n", curr_rank);
            sum += curr_rank;
        }
        printf("Sum of ranks: %d\n", sum);
    }

    MPI_Finalize();
    return 0;
}
