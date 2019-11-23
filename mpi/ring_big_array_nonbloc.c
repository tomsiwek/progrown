#include <mpi.h>
#include <stdio.h>

#define maxsize 100000

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);
        
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int numranks;
    MPI_Comm_size(MPI_COMM_WORLD, &numranks);
    int src, dest;
    int tag = 42;
    MPI_Status status_send, status_recv;
    MPI_Request req_send;
    int array[maxsize];
    int received[maxsize];

    /* init the array */
    int i;
    for(i = 0; i < maxsize; i++)
    {
        array[i] = i*rank;
    }
 
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

    printf("Process %d begins senging an array of %d elements\n", rank, maxsize);

    MPI_Isend(array, maxsize, MPI_INT, dest, tag, MPI_COMM_WORLD, &req_send);
    MPI_Recv(received, maxsize, MPI_INT, src, tag, MPI_COMM_WORLD, &status_recv);
    MPI_Wait(&req_send, &status_send);
        
    printf("Process %d has received an array from process %d, stored at adress %X of %d elements\n", rank, status_recv.MPI_SOURCE, received, sizeof(received)/sizeof(int));

    MPI_Finalize();
    return 0;
}
