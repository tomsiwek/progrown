#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank_world;
    MPI_Comm ring;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_world);

    int dim[] = {4};
    int period[] = {1};
    MPI_Cart_create(MPI_COMM_WORLD, 1, dim, period, 0, &ring);
    int rank;
    MPI_Comm_rank(ring, &rank);
    int coord[1];

    int tag = 42;
    MPI_Status status;
    MPI_Request req;
    char mess[50];
 
    sprintf(mess, "A message from the process No %d", rank);

    int computed;
    int prev, next;
    MPI_Cart_coords(ring, rank, 1, coord);
    MPI_Cart_rank(ring, coord, &computed);
    MPI_Cart_shift(ring, 0, 1, &prev, &next);
    printf("my rank: %d, ", rank);
    printf("next process: %d, ", next);
    printf("previous process: %d\n", prev);
    MPI_Isend(mess, strlen(mess) + 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD, &req);
    MPI_Recv(mess, 50, MPI_CHAR, prev, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Wait(&req, &status);
    printf("Process No %d has received a message: %s\n", rank, mess);

    MPI_Comm_free(&ring);
    MPI_Finalize();
    return 0;
}
