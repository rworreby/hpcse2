// File       : fpeak.cu
// Created    : Sat May 01 2021 09:57:06 AM (+0200)
// Description: Floating point peak performance kernel
// Copyright 2021 ETH Zurich. All Rights Reserved.

#include "CUDATimer.cuh"
#include <stdio.h>

typedef float Real;

// TODO: Define the nominal floating point performance of your target GPU in
// gigaflop
#define PMAX (1)

// TODO: Define a GPU kernel to attain peak floating point performance

int main(void)
{
    GPUtimer timer;
    timer.start();

    // TODO: Invoke your kernel here

    timer.stop();

    cudaDeviceSynchronize();

    // TODO: Compute the number of gigaflop your kernel has performed within
    // the measured time
    const double Gflop = 1;

    const double kTime = timer.elapsed() * 1.0e-3; // seconds
    const double Perf = Gflop / kTime; // Gflops
    const double frac = Perf / PMAX;   // fraction of peak
    printf("Performance: %f Gflops (%4.1f%% of Peak)\n", Perf, frac * 100);
    printf("Kernel Time: %f s\n", kTime);

    return 0;
}
