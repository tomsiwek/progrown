#include <mpi.h>
#include <stdio.h>

void print_matrix(int m[4][4])
{
    for (int x = 0; x < 4; x++)
    {
        for (int y = 0; y < 4; y++)
        {
            printf ("%d ", m[x][y]);
        }
        printf ("\n");
    }
}

int main(int argc, char *argv[])
{
    int rank;
    int N = 4;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Datatype diag;
    MPI_Type_vector(N, 1, N+1, MPI_INT, &diag);
    MPI_Type_commit(&diag);

    if(rank == 0)
    {
        int matrix[4][4] = { { 1,   2,  3,  4},
                             { 5,   6,  7,  8}, 
                             { 9,  10, 11, 12},
                             {13,  14, 15, 16} };
        print_matrix (matrix);
        printf("\n");
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Send(matrix, 1, diag, 1, 123, MPI_COMM_WORLD);
    }
    else
    { 
        int matrix[4][4] = { { -1, -1, -1, -1 },
                             { -2, -2, -2, -2 },
                             { -3, -3, -3, -3 },
                             { -4, -4, -4, -4 } };
        printf("\n");
        MPI_Barrier(MPI_COMM_WORLD);
        print_matrix(matrix);
        printf("\n");
        MPI_Recv(matrix, 1, diag, 0, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        print_matrix(matrix);
    }

    MPI_Type_free(&diag);
    MPI_Finalize();
    return 0;
}
