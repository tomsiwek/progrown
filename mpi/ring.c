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
 
    sprintf(mess, "A message from the process No %d", rank);

    if(rank == 0) /* proces o zerowej randze */
    {
        dest = 1;
	src = (numranks - 1);
    }
    else if(rank == (numranks - 1)) /* proces o najwyższej randze */
    {
        dest = 0;
        src = (rank - 1);
    }
    else /* pozostałe procesy */
    {
        dest = (rank + 1);
        src = (rank - 1);
    }

    MPI_Send(mess, strlen(mess) + 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
    MPI_Recv(mess, 50, MPI_CHAR, src, tag, MPI_COMM_WORLD, &status);
        
    printf("Process %d has received a message: %s\n", rank, mess);

    MPI_Finalize();
    return 0;
}
