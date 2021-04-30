// File       : LaplacianSmootherMPI.h
// Created    : Wed Apr 14 2021 12:21:16 PM (+0200)
// Description: Laplacian data smoother (distributed)
// Copyright 2021 ETH Zurich. All Rights Reserved.
#ifndef LAPLACIANSMOOTHERMPI_H_XE8EG6D0
#define LAPLACIANSMOOTHERMPI_H_XE8EG6D0

#include "LaplacianSmoother.h"
#include <mpi.h>
#include <vector>

#ifdef _SINGLE_PRECISION_
#define MPIDataType MPI_FLOAT
#else
#define MPIDataType MPI_DOUBLE
#endif /* _SINGLE_PRECISION_ */

class LaplacianSmootherMPI : public LaplacianSmoother
{
public:
    LaplacianSmootherMPI(const int Nx = 128, // per process
                         const int Ny = 128, // per process
                         const int Nz = 128, // per process
                         const int Px = 1,   // processes in x
                         const int Py = 1,   // processes in y
                         const int Pz = 1,   // processes in z
                         const MPI_Comm comm_root = MPI_COMM_WORLD);

    ~LaplacianSmootherMPI();

    // report measurements (distributed)
    void report() const override;

    // distributed dump data
    void dump(const std::string fname) const override
    {
        // to check our MPI implementation we dump a binary file only when we
        // run the MPI code with one rank.  To check if your implementation is
        // correct, you can generate a reference file from the sequential code
        // and one from the MPI code.  If your ghost cell exchange is
        // implementation correctly, then the following check must tell you that
        // the two files are an equivalent binary representation:
        //
        // diff referenceMPI.bin referenceSEQ.bin
        //
        if (1 == procs_[0] * procs_[1] * procs_[2]) {
            LaplacianSmoother::dump(fname);
        }
    }

protected:
    // communication methods
    void comm_() override;
    void sync_() override;

private:
    enum { X0, X1, Y0, Y1, Z0, Z1, NFaces }; // neighbor id's for convenience

    // MPI topology
    MPI_Comm comm_world_, comm_cart_;
    const int procs_[3];
    int rank_cart_, nbr_[NFaces];

    // MPI datatypes
    MPI_Datatype StripeX, FaceX, FaceY, FaceZ;
    std::vector<MPI_Request> recv_req_;
    std::vector<MPI_Request> send_req_;
};

#endif /* LAPLACIANSMOOTHERMPI_H_XE8EG6D0 */
