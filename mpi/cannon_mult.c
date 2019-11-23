#include <mpi.h>
#include <stdio.h>

#define MATRIXDIM 3

//https://iq.opengenus.org/cannon-algorithm-distributed-matrix-multiplication/

int main(int argc, char* argv[])
{
    /* Initialise MPI */
    int tag = 44;
    int rank_world;
    double a, b, c = 0.0;
    int row, col;
    MPI_Comm grid;
    MPI_Request req[2];
    MPI_Status status[2];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_world);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    /* Initialise Cartesian 2-D grid. */
    int dims[] = {MATRIXDIM, MATRIXDIM};
    int periods[] = {1, 1};
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &grid);
    int rank;
    MPI_Comm_rank(grid, &rank);
    int coords[2];
   
    MPI_Cart_coords(grid, rank, 2, coords);

    int prev[2], next[2];
    MPI_Cart_shift(grid, 1, 1, &prev[0], &next[0]);
    MPI_Cart_shift(grid, 0, 1, &prev[1], &next[1]);

    /* Initialise, shuffle and scatter input matrices. */
    if(rank == 0)
    {
        double mat_a[MATRIXDIM][MATRIXDIM] = {{1, 7, 9}, {4, 2, 8}, {6, 5, 3}};
        double mat_b[MATRIXDIM][MATRIXDIM] = {{1, 4, 5}, {2, 1, 4}, {3, 2, 1}};
        //double mat_a[MATRIXDIM][MATRIXDIM] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
        //double mat_b[MATRIXDIM][MATRIXDIM] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

        double row_col_tmp[MATRIXDIM]; 

        /* Shuffle first matrix row-wise*/
        for(row = 1; row < MATRIXDIM; row++)
        {
            memcpy(row_col_tmp, &mat_a[row], MATRIXDIM*sizeof(double));
            for(col = 0; col < MATRIXDIM; col++)
            {
                mat_a[row][col] = row_col_tmp[(col + row)%MATRIXDIM];
            }
        }

        /* Shuffle second matrix column-wise */
        for(col = 1; col < MATRIXDIM; col++)
        {
            for(row = 0; row < MATRIXDIM; row++)
            {
                row_col_tmp[row] = mat_b[row][col];
            }
            for(row = 0; row < MATRIXDIM; row++)
            {
                mat_b[row][col] = row_col_tmp[(col + row)%MATRIXDIM];
            }
        }

        MPI_Scatter(&mat_a, 1, MPI_DOUBLE, &a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Scatter(&mat_b, 1, MPI_DOUBLE, &b, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Scatter(NULL, 0, NULL, &a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Scatter(NULL, 0, NULL, &b, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    //MPI_Barrier(grid);
    
    int l;
    for(l = 0; l <= (MATRIXDIM - 1); l++)
    {
        c += a*b;
        MPI_Sendrecv_replace(&a, 1, MPI_DOUBLE, prev[0], tag, next[0], tag, grid, MPI_STATUS_IGNORE);
        MPI_Sendrecv_replace(&b, 1, MPI_DOUBLE, prev[1], tag, next[1], tag, grid, MPI_STATUS_IGNORE);
    }

    MPI_Barrier(grid);

    if(rank != 0)
    {
        MPI_Gather(&c, 1, MPI_DOUBLE, NULL, 0, NULL, 0, grid);
    }
    else
    {
        double mat_c[MATRIXDIM][MATRIXDIM];
        MPI_Gather(&c, 1, MPI_DOUBLE, &mat_c, 1, MPI_DOUBLE, 0, grid);
        for(row = 0; row < MATRIXDIM; row++)
        {
            for(col = 0; col < MATRIXDIM; col++)
            {
                printf("%8.2f ", mat_c[row][col]);
            }
            printf("\n");
        }
    }

    MPI_Finalize();	
    return 0;
}
