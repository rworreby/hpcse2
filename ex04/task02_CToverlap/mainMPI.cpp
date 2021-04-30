// File       : main.cpp
// Created    : Thu Apr 15 2021 09:40:20 AM (+0200)
// Description: Distributed data smoothing application
// Copyright 2021 ETH Zurich. All Rights Reserved.

#include <LaplacianSmootherMPI.h>
#include <cstdlib>

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    {
        int Nx, Ny, Nz, Px, Py, Pz;
        Nx = Ny = Nz = 128;
        Px = Py = Pz = 1;
        if (7 == argc) {
            Nx = std::atoi(argv[1]);
            Ny = std::atoi(argv[2]);
            Nz = std::atoi(argv[3]);
            Px = std::atoi(argv[4]);
            Py = std::atoi(argv[5]);
            Pz = std::atoi(argv[6]);
        }

        LaplacianSmootherMPI ls_data(Nx, Ny, Nz, Px, Py, Pz, MPI_COMM_WORLD);
        for (int i = 0; i < 100; ++i) {
            ls_data.sweep();
        }
        ls_data.dump("referenceMPI.bin");
        ls_data.report();
    }

    MPI_Finalize();
    return 0;
}
