// File       : LaplacianSmootherMPI.cpp
// Created    : Wed Apr 14 2021 03:01:05 PM (+0200)
// Description: Laplacian smoother implementation (distributed)
// Copyright 2021 ETH Zurich. All Rights Reserved.

#include "LaplacianSmootherMPI.h"
#include <chrono>

using namespace std::chrono;

LaplacianSmootherMPI::LaplacianSmootherMPI(const int Nx,
                                           const int Ny,
                                           const int Nz,
                                           const int Px,
                                           const int Py,
                                           const int Pz,
                                           const MPI_Comm comm_root)
    : LaplacianSmoother(Nx, Ny, Nz), comm_world_(comm_root),
      comm_cart_(MPI_COMM_NULL), procs_{Px, Py, Pz},
      recv_req_(6, MPI_REQUEST_NULL), send_req_(6, MPI_REQUEST_NULL)
{
    int size;
    MPI_Comm_size(comm_world_, &size);
    if (size != Px * Py * Pz) {
        MPI_Abort(comm_world_, 1);
    }
    // TODO: Define a periodic Cartesian MPI topology

    // TODO: Create derived MPI datatypes to communicate the ghost cells on each
    // of the 6 faces
}

LaplacianSmootherMPI::~LaplacianSmootherMPI()
{
    // TODO: You may need to free allocated resources
}

void LaplacianSmootherMPI::report() const
{
    // TODO: Gather the time measurements of each rank and report the minimum,
    // maximum and average time measured for:
    // t_sweep_ (accumulated time for sweep_count_ smooth sweeps)
    // t_async_ (accumulated time spent in comm_(); see LaplacianSmoother.cpp:27)
    // t_inner_ (accumulated time spent to compute internal domain; see LaplacianSmoother.cpp:28)
    // t_ghosts_ (accumulated time spent in sync_(); see LaplacianSmoother.cpp:29)
    // t_boundary_ (accumulated time spent to compute data on boundary; see LaplacianSmoother.cpp:30)
}

void LaplacianSmootherMPI::comm_()
{
    // TODO: Initiate asynchronous communication of the ghost cells on the 6
    // faces with your neighboring ranks.  It is allowed to communicate with
    // oneself in MPI.
}

void LaplacianSmootherMPI::sync_()
{
    // TODO: Ensure that your asynchronous communication has successfully
    // concluded such that you can continue to process the boundary data.
}
