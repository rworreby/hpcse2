#include <iostream>
#include "mpi.h"

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (size_t i = 1; i <= 16777216; i*=2) {
        double *buf = (double *) malloc( i * sizeof(double) );
        //if (!rank) std::cout << "Running measurements for " << i << '\n';

        int loop_repetitions = 1024 * 1024 / i;
        if (loop_repetitions < 1){
            loop_repetitions = 1;
        }


        double starttime = MPI_Wtime();

        for (size_t k = 0; k < loop_repetitions; k++) {

            if (rank == 0) {
                MPI_Send(
                    buf,
                    i,
                    MPI_C_BOOL,
                    1,
                    1,
                    MPI_COMM_WORLD
                );
            }

            if (rank == 1) {
                MPI_Recv(
                    buf, // buffer
                    i, // count
                    MPI_C_BOOL, // datatype
                    0, // source
                    1, //tag
                    MPI_COMM_WORLD, // communicator
                    MPI_STATUS_IGNORE
                );
            }
        }

        double endtime = MPI_Wtime();
        if (!rank) {
            double time = (endtime - starttime) / loop_repetitions;
            std::cout << i << "," << time << std::endl;
        }
    }

    MPI_Finalize();
    return 0;
}
