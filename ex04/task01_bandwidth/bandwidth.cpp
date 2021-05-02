#include <iostream>
#include "mpi.h"

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (size_t i = 1; i <= 16777216; i*=2) {.
        double *buf = (double *) malloc( i * sizeof(double) );
        std::cout << "Running measurements for " << i << '\n';
        double starttime = MPI_Wtime();

        int loop_repetitions = 1024 / n;
        if (loop_repetitions < 1){
            loop_repetitions = 1;
        }

        for (size_t k = 0; k < loop_repetitions; k++) {

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

            if (rank == 1) {
                MPI_Recv(
                    buf, // buffer
                    i, // count
                    MPI_BOOL, // datatype
                    0, // source
                    1, //tag
                    MPI_COMM_WORLD // communicator
                )
            }
        }

        double endtime = MPI_Wtime();
        double time = (endtime - starttime) / loop_repetitions;
        std::cout << i << "," << time << std::endl;
    }

    return 0;
}
