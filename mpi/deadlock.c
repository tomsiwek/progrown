#include <mpi.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    int rank;
    MPI_Init(&argc, &argv); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int tag = 44;
    int send = 5;
    int recv;
    
    if(rank == 0)
    {
        /*MPI_Request req;*/
        MPI_Ssend(&send, 1, MPI_CHAR, 1, tag, MPI_COMM_WORLD);
        MPI_Recv(&recv, 1, MPI_CHAR, 1, tag + 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        /*MPI_Isend(mess, strlen(mess) + 1, MPI_CHAR, 1, rank, MPI_COMM_WORLD, &req);
        MPI_Wait(&req, MPI_STATUS_IGNORE);
        MPI_Irecv(recv, strlen(recv) + 1, MPI_CHAR, 1, rank, MPI_COMM_WORLD, &req);*/
    }
    else
    {
        /*MPI_Request req;*/
        MPI_Recv(&recv, 1, MPI_CHAR, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Ssend(&send, 1, MPI_CHAR, 0, tag + 1, MPI_COMM_WORLD);
        /*MPI_Isend(mess, strlen(mess) + 1, MPI_CHAR, 0, rank, MPI_COMM_WORLD, &req);
        MPI_Wait(&req, MPI_STATUS_IGNORE);
        MPI_Irecv(recv, strlen(recv) + 1, MPI_CHAR, 0, rank, MPI_COMM_WORLD, &req);*/
    }

    printf("Received value: %d\n", recv);

    MPI_Finalize();

    return 0;

}
