#include "wave.hpp"
#include <cassert>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 4) {
        if (rank == 0) {
            // clang-format off
            printf("==============================================================\n");
            printf("Wrong arguments! Correct usage:\n");
            printf("mpirun -n np wave {gridpoints_per_dim} {procs_per_dim} {t_end}\n");
            printf("==============================================================\n");
            // clang-format on
        }
    }

    size_t gridpoints_per_dim = atoi(argv[1]);
    size_t procs_per_dim = atoi(argv[2]);
    double t_end = atoi(argv[3]);
    assert(gridpoints_per_dim % procs_per_dim == 0);
    assert(procs_per_dim * procs_per_dim == size);

    {
        WaveEquation simulation = WaveEquation(
            gridpoints_per_dim, procs_per_dim, t_end, MPI_COMM_WORLD);
        simulation.run();
    }

    MPI_Finalize();

    return 0;
}
