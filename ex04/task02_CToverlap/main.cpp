// File       : main.cpp
// Created    : Thu Apr 15 2021 09:40:20 AM (+0200)
// Description: Data smoothing application
// Copyright 2021 ETH Zurich. All Rights Reserved.

#include <LaplacianSmoother.h>
#include <cstdlib>

int main(int argc, char *argv[])
{
    int Nx, Ny, Nz;
    Nx = Ny = Nz = 128;
    if (4 == argc) {
        Nx = std::atoi(argv[1]);
        Ny = std::atoi(argv[2]);
        Nz = std::atoi(argv[3]);
    }

    LaplacianSmoother ls_data(Nx, Ny, Nz);
    for (int i = 0; i < 100; ++i) {
        ls_data.sweep();
    }
    ls_data.dump("referenceSEQ.bin");
    ls_data.report();

    return 0;
}
