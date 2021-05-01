#include <iostream>
#include "mpi.h"

int main(int argc, char **argv)
{
    double *buf = (double *) malloc( n * sizeof(double) );

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    for (size_t i = 1; i <= 16777216; i*=2) {
        std::cout << "Running measurements for " << i << '\n';
        starttime = MPI_Wtime();


        if (rank == 0) {
            MPI_Send(
                buf,
                i,
                MPI_BOOL,
                1,
                1,
                MPI_COMM_WORLD
            )

        }


        endtime = MPI_Wtime();
        std::cout << i << "," << (endtime - starttime) << std::endl;

    }



    return 0;
}
