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
    char mess[50];

    if(rank != 0) /* procesy o niezerowej randze */
    {
        sprintf(mess, "A message from the process No %d", rank);
        dest = 0;
        MPI_Send(mess, strlen(mess) + 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
    }
    else /* proces o randze zero */
    {
        for(src = 1; src < numranks; src++)
        {
            MPI_Recv(mess, 50, MPI_CHAR, src, tag, MPI_COMM_WORLD, &status);
 
            printf("%s\n", mess);
        }
    }

    MPI_Finalize();
    return 0;
}
