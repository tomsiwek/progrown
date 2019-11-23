#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

double int_fun(double x)
{
    return 4.0/(1.0 + x*x);
}

int main(int argc, char* argv[])
{
    typedef struct
    {
        double sub_sum;
        double time;
    } result_str;
    result_str result;
    MPI_Datatype res_str;
    int i, j;
    short cpus = atoi(argv[1]);
    const int N = atoi(argv[2]);
    const int iter = N/cpus;
    double sub_sum = 0.0;
    const double h = 1.0/N;
    double sum = 0.0;
    int rank;
    double start;
    int tag = 76;
    MPI_Status status;
    int numranks;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numranks);
    
    int sizes[2] = {1, 1};
    MPI_Aint addr_str, addr_time;
    MPI_Get_address(&result, &addr_str);
    MPI_Get_address(&result.time, &addr_time);
    MPI_Aint disps[2] = {0, MPI_Aint_diff(addr_time, addr_str)};
    MPI_Datatype types[2] = {MPI_DOUBLE, MPI_DOUBLE};
    MPI_Type_create_struct(2, sizes, disps, types, &res_str);
    MPI_Type_commit(&res_str);

    MPI_Barrier(MPI_COMM_WORLD); /* IMPORTANT */
    start = MPI_Wtime();

    for(i = 0; i < iter; i++)
    {
        sub_sum += int_fun(((i + rank*iter) + 0.5)/N)*h;
    }

    /*printf("subtotal: %.16f\n", sub_sum);*/

    result.sub_sum = sub_sum;
    result.time = MPI_Wtime() - start;

    if(rank != 0)
    {
        MPI_Send(&result, 1, res_str, 0, tag, MPI_COMM_WORLD);
    }
    else
    {
	sum += result.sub_sum;
	printf("Master process %d calculated sub sum %f in time %f s\n", rank, result.sub_sum, result.time);
        for(j = 1; j < numranks; j++)
	{
            MPI_Recv(&result, 1, res_str, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
	    sum += result.sub_sum;
            printf("Worker process %d calculated sub sum %f in time %f s\n", status.MPI_SOURCE, result.sub_sum, result.time);

	}

	printf("pi=%.16f\n", sum);
    }

    MPI_Type_free(&res_str);

    MPI_Finalize();

    return 0;
}
