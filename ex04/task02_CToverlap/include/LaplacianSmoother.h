// File       : LaplacianSmoother.h
// Created    : Wed Apr 14 2021 12:21:16 PM (+0200)
// Description: Laplacian data smoother
// Copyright 2021 ETH Zurich. All Rights Reserved.
#ifndef LAPLACIANSMOOTHER_H_YPLTRNTS
#define LAPLACIANSMOOTHER_H_YPLTRNTS

#include <cassert>
#include <string>
#include <vector>

class LaplacianSmoother
{
public:
    LaplacianSmoother(const int Nx = 128,
                      const int Ny = 128,
                      const int Nz = 128)
        : sweep_count_(0), t_sweep_(0.0), t_async_(0.0), t_inner_(0.0),
          t_ghosts_(0.0),
          t_boundary_(0.0), N_{Nx + 2, Ny + 2, Nz + 2}, // +2 for ghost cells
          buf0_(N_[0] * N_[1] * N_[2]), buf1_(buf0_.size())
    {
        // setup source and destination data pointer
        data_ = buf0_.data();
        temp_ = buf1_.data();

        // initialize data
        initialize_();
    }

    virtual ~LaplacianSmoother() {}

#ifdef _SINGLE_PRECISION_
    using DataType = float;
#else
    using DataType = double;
#endif /* _SINGLE_PRECISION_ */

    // perform a smoothing sweep
    void sweep();

    // report measurements
    virtual void report() const;

    // dump data
    virtual void dump(const std::string fname) const;

    // data accessors
    const DataType &operator()(const int ix, const int iy, const int iz) const
    {
        // stencil is +/- 1:
        const int I = ix + 1;
        const int J = iy + 1;
        const int K = iz + 1;
        assert(I < N_[0]);
        assert(J < N_[1]);
        assert(K < N_[2]);
        return data_[I + N_[0] * (J + N_[1] * K)];
    }

    DataType &operator()(const int ix, const int iy, const int iz)
    {
        // stencil is +/- 1:
        const int I = ix + 1;
        const int J = iy + 1;
        const int K = iz + 1;
        assert(I < N_[0]);
        assert(J < N_[1]);
        assert(K < N_[2]);
        return data_[I + N_[0] * (J + N_[1] * K)];
    }

protected:
    // profiling
    size_t sweep_count_;
    double t_sweep_, t_async_, t_inner_, t_ghosts_, t_boundary_;

    // data dimensions (including ghost cells)
    const int N_[3];

    // main data buffers
    std::vector<DataType> buf0_;
    std::vector<DataType> buf1_;
    DataType *data_; // data reference
    DataType *temp_; // temporary reference

    // communication methods
    virtual void comm_() {}
    virtual void sync_();

private:
    void smooth_inner_();
    void smooth_boundary_();

    DataType &update_(const int ix, const int iy, const int iz)
    {
        // stencil is +/- 1:
        const int I = ix + 1;
        const int J = iy + 1;
        const int K = iz + 1;
        assert(I < N_[0]);
        assert(J < N_[1]);
        assert(K < N_[2]);
        return temp_[I + N_[0] * (J + N_[1] * K)];
    }

    void initialize_();
};

#endif /* LAPLACIANSMOOTHER_H_YPLTRNTS */
