// File       : LaplacianSmoother.cpp
// Created    : Wed Apr 14 2021 03:01:05 PM (+0200)
// Description: Laplacian smoother implementation
// Copyright 2021 ETH Zurich. All Rights Reserved.

#include "LaplacianSmoother.h"
#include <chrono>
#include <cstdio>
#include <fstream>
#include <random>

using namespace std::chrono;

#define MEASURE(func, timer)                                                   \
    do {                                                                       \
        const auto t_start = steady_clock::now();                              \
        func;                                                                  \
        const auto t_end = steady_clock::now();                                \
        timer += duration_cast<microseconds>(t_end - t_start).count();         \
    } while (0)

void LaplacianSmoother::sweep()
{
    const auto ts_start = steady_clock::now();

    // smoothing logic:
    MEASURE(comm_(), t_async_);
    MEASURE(smooth_inner_(), t_inner_);
    MEASURE(sync_(), t_ghosts_);
    MEASURE(smooth_boundary_(), t_boundary_);

    // swap references
    std::swap(temp_, data_);

    sweep_count_ += 1;
    const auto ts_end = steady_clock::now();
    t_sweep_ += duration_cast<microseconds>(ts_end - ts_start).count();
}

void LaplacianSmoother::report() const
{
    const int Nx = N_[0] - 2;
    const int Ny = N_[1] - 2;
    const int Nz = N_[2] - 2;
    const double t_sweep = t_sweep_ / sweep_count_;
    const double t_async = t_async_ / sweep_count_;
    const double t_inner = t_inner_ / sweep_count_;
    const double t_ghosts = t_ghosts_ / sweep_count_;
    const double t_boundary = t_boundary_ / sweep_count_;
    // clang-format off
    std::printf("Measurement report for dimension %d x %d x %d:\n", Nx, Ny, Nz);
    std::printf("\tSweep count:           %zu\n", sweep_count_);
    std::printf("\tAvg. time sweep:       %.3e microseconds\n", t_sweep);
    std::printf("\tAvg. time async comm:  %.3e microseconds (%.2f%%)\n", t_async, 100*t_async/t_sweep);
    std::printf("\tAvg. time inner:       %.3e microseconds (%.2f%%)\n", t_inner, 100*t_inner/t_sweep);
    std::printf("\tAvg. time sync ghosts: %.3e microseconds (%.2f%%)\n", t_ghosts, 100*t_ghosts/t_sweep);
    std::printf("\tAvg. time boundary:    %.3e microseconds (%.2f%%)\n", t_boundary, 100*t_boundary/t_sweep);
    // clang-format on
}

void LaplacianSmoother::dump(const std::string fname) const
{
    // dump the domain without ghosts
    const int Nx = N_[0] - 2;
    const int Ny = N_[1] - 2;
    const int Nz = N_[2] - 2;
    if (128 != Nx && 128 != Ny && 128 != Nz) {
        // only create binary dump for default setup (for testing)
        return;
    }
    std::ofstream out(fname, std::ios_base::binary);

    // small header
    const int prec = sizeof(DataType);
    out.write(reinterpret_cast<const char *>(&prec), sizeof(int));
    out.write(reinterpret_cast<const char *>(&Nx), sizeof(int));
    out.write(reinterpret_cast<const char *>(&Ny), sizeof(int));
    out.write(reinterpret_cast<const char *>(&Nz), sizeof(int));

    // data
    for (int k = 0; k < Nz; ++k) {
        for (int j = 0; j < Ny; ++j) {
            const char *src =
                reinterpret_cast<const char *>(&operator()(0, j, k));
            out.write(src, Nx * sizeof(DataType));
        }
    }
}

void LaplacianSmoother::sync_()
{
    // no synchronization needed here, only loading (periodic) ghost cells is
    // needed
    const int Nx = N_[0] - 2;
    const int Ny = N_[1] - 2;
    const int Nz = N_[2] - 2;

    // j-k faces
    for (int k = 0; k < Nz; ++k) {
        for (int j = 0; j < Ny; ++j) {
            operator()(-1, j, k) = operator()(Nx - 1, j, k);
            operator()(Nx, j, k) = operator()(0, j, k);
        }
    }

    // i-k faces
    for (int k = 0; k < Nz; ++k) {
        for (int i = 0; i < Nx; ++i) {
            operator()(i, -1, k) = operator()(i, Ny - 1, k);
            operator()(i, Ny, k) = operator()(i, 0, k);
        }
    }

    // i-j faces
    for (int j = 0; j < Ny; ++j) {
        for (int i = 0; i < Nx; ++i) {
            operator()(i, j, -1) = operator()(i, j, Nz - 1);
            operator()(i, j, Nz) = operator()(i, j, 0);
        }
    }
}

void LaplacianSmoother::smooth_inner_()
{
    // smooth inner domain
    const int Nx = N_[0] - 2;
    const int Ny = N_[1] - 2;
    const int Nz = N_[2] - 2;
    const DataType fac = 1.0 / 12.0;
#pragma omp parallel for
    for (int k = 1; k < Nz - 1; ++k) {
        for (int j = 1; j < Ny - 1; ++j) {
            for (int i = 1; i < Nx - 1; ++i) {
                // clang-format off
                update_(i, j, k) = fac * (
                    operator()(i - 1, j, k) +
                    operator()(i + 1, j, k) +
                    operator()(i, j - 1, k) +
                    operator()(i, j + 1, k) +
                    operator()(i, j, k - 1) +
                    operator()(i, j, k + 1) +
                    6 * operator()(i, j, k));
                // clang-format on
            }
        }
    }
}

void LaplacianSmoother::smooth_boundary_()
{
    // smooth boundary domain
    const int Nx = N_[0] - 2;
    const int Ny = N_[1] - 2;
    const int Nz = N_[2] - 2;
    const DataType fac = 1.0 / 12.0;

    // j-k faces
    const int i0 = 0;
    const int iN = Nx - 1;
#pragma omp parallel for
    for (int k = 0; k < Nz; ++k) {
        for (int j = 0; j < Ny; ++j) {
            // clang-format off
            update_(i0, j, k) = fac * (
                operator()(i0 - 1, j, k) +
                operator()(i0 + 1, j, k) +
                operator()(i0, j - 1, k) +
                operator()(i0, j + 1, k) +
                operator()(i0, j, k - 1) +
                operator()(i0, j, k + 1) +
                6 * operator()(i0, j, k));

            update_(iN, j, k) = fac * (
                operator()(iN - 1, j, k) +
                operator()(iN + 1, j, k) +
                operator()(iN, j - 1, k) +
                operator()(iN, j + 1, k) +
                operator()(iN, j, k - 1) +
                operator()(iN, j, k + 1) +
                6 * operator()(iN, j, k));
            // clang-format on
        }
    }

    // i-k faces
    const int j0 = 0;
    const int jN = Ny - 1;
#pragma omp parallel for
    for (int k = 0; k < Nz; ++k) {
        for (int i = 1; i < Nx - 1; ++i) {
            // clang-format off
            update_(i, j0, k) = fac * (
                operator()(i - 1, j0, k) +
                operator()(i + 1, j0, k) +
                operator()(i, j0 - 1, k) +
                operator()(i, j0 + 1, k) +
                operator()(i, j0, k - 1) +
                operator()(i, j0, k + 1) +
                6 * operator()(i, j0, k));

            update_(i, jN, k) = fac * (
                operator()(i - 1, jN, k) +
                operator()(i + 1, jN, k) +
                operator()(i, jN - 1, k) +
                operator()(i, jN + 1, k) +
                operator()(i, jN, k - 1) +
                operator()(i, jN, k + 1) +
                6 * operator()(i, jN, k));
            // clang-format on
        }
    }

    // i-j faces
    const int k0 = 0;
    const int kN = Nz - 1;
#pragma omp parallel for
    for (int j = 1; j < Ny - 1; ++j) {
        for (int i = 1; i < Nx - 1; ++i) {
            // clang-format off
            update_(i, j, k0) = fac * (
                operator()(i - 1, j, k0) +
                operator()(i + 1, j, k0) +
                operator()(i, j - 1, k0) +
                operator()(i, j + 1, k0) +
                operator()(i, j, k0 - 1) +
                operator()(i, j, k0 + 1) +
                6 * operator()(i, j, k0));

            update_(i, j, kN) = fac * (
                operator()(i - 1, j, kN) +
                operator()(i + 1, j, kN) +
                operator()(i, j - 1, kN) +
                operator()(i, j + 1, kN) +
                operator()(i, j, kN - 1) +
                operator()(i, j, kN + 1) +
                6 * operator()(i, j, kN));
            // clang-format on
        }
    }
}

void LaplacianSmoother::initialize_()
{
    const int Nx = N_[0] - 2;
    const int Ny = N_[1] - 2;
    const int Nz = N_[2] - 2;
    std::mt19937 gen(101);
    std::uniform_int_distribution<int> U(0, 1);
    for (int k = 0; k < Nz; ++k) {
        for (int j = 0; j < Ny; ++j) {
            for (int i = 0; i < Nx; ++i) {
                operator()(i, j, k) = U(gen);
            }
        }
    }
}
