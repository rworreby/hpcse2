#include <cmath>
#include <iomanip>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "wave.hpp"

//
// Function defining the initial displacement on the grid
//
double f(double x, double y)
{
    double r = (x - 0.5) * (x - 0.5) + (y - 0.5) * (y - 0.5);
    return 0.3 * exp(-5.0 * r / 0.1);
}

WaveEquation::WaveEquation(size_t a_gridpoints_per_dim,
                           size_t a_procs_per_dim,
                           double a_t_end,
                           MPI_Comm a_comm)
    : Ntot(a_gridpoints_per_dim), procs_per_dim(a_procs_per_dim),
      t_end(a_t_end), cart_comm(a_comm)
{
    h = L / Ntot;
    N = Ntot / procs_per_dim;
    size_t N_halo = N + 2;

    dt = h / sqrt(3.0);
    c_aug = dt * dt / (h * h);

    u.resize(N_halo * N_halo);
    u_old.resize(N_halo * N_halo);
    u_new.resize(N_halo * N_halo);

    // TODO Question a)
    // MPI related initializations (create new communicator and initialize
    // member variables) (see 'wave.hpp' for present member variables:
    // cart_comm, nums, rank_plus, rank_minus)
    //
    // Do not forget to free the communicator in the destructor of this class!!
    // (bottom of this file)

    if (rank == 0) {
        printf("(%d, %d) processes mapping to a (%d, %d) grid\n",
               nums[0],
               nums[1],
               N,
               N);
    }

    //
    // Find its location in the simulation space
    // TODO Uncomment this section and the one at the beginning when finished
    // with subquestion a))
    //

    /*
    MPI_Cart_coords(cart_comm, rank, 2, &coords[0]);
    origin[0] = N * h * coords[0];
    origin[1] = N * h * coords[1];
    */

    //
    // Set initial conditions (inclusive first time derivative)
    //
    initializeGrid();

    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            size_t idx = (i + 1) * (N + 2) + (j + 1);
            u_old[idx] = u[idx];
            u_new[idx] = u[idx];
        }
    }
}

void WaveEquation::run()
{
    double t = 0.0;
    unsigned int count = 0;

    // TODO Question b)
    // Create datatypes to communicate halo boundaries (before the loop while-loop)
    //
    // You can use the buffers below to create the necessary array datatypes in a for-loop.
    // Feel free to initialize and store them in a different format
    // (this might be less error prone but also results in more lines of code).
    //
    // The buffers' lengths represent the number of dimensions of our domain (we are solving a 2D problem).
    // i.e. in SEND_HALO_PLUS[0] we store the datatype for sending the right-most column
    //          (of the inner N x N grid) to the right neighboring rank.
    //      RECV_HALO_MINUS[1] stores the datatype for receiving the halo row sent from the rank below.
    //  Contact the slides of the tutorial where we introduced the exercise if anything is unclear.

    MPI_Datatype SEND_HALO_PLUS[2];
    MPI_Datatype SEND_HALO_MINUS[2];

    MPI_Datatype RECV_HALO_PLUS[2];
    MPI_Datatype RECV_HALO_MINUS[2];

    //
    // Main loop propagating the solution forward in time
    //
    while (t < t_end) {
        // TODO Question c)
        // Send and receive halo boundaries
        //

        //
        // TODO Uncomment following when finished with subquestion a)
        //
        /*
        if(count%10 == 0){
            saveGrid(count);
            double l2_norm = computeSquaredL2Norm();
            if(rank == 0)
                printf("t=%i : L2-Norm = %f\n", count, l2_norm);
        }
        */

        //
        // Update the cells with FD stencil
        //
        for (size_t i = 1; i < N + 1; ++i) {
            for (size_t j = 1; j < N + 1; ++j) {
                applyStencil(i, j);
            }
        }

        //
        // Swap vectors
        //
        u_old.swap(u);
        u.swap(u_new);

        //
        // Update time
        //
        t += dt;
        count++;
    }

    // TODO Question c)
    // Free communication datatypes
    //
}

void WaveEquation::initializeGrid()
{
    double x_pos, y_pos;
    for (size_t i = 0; i < N; ++i) {
        x_pos = origin[0] + i * h + 0.5 * h;
        for (size_t j = 0; j < N; ++j) {
            y_pos = origin[1] + j * h + 0.5 * h;
            u[(i + 1) * (N + 2) + (j + 1)] = f(x_pos, y_pos);
        }
    }
}

double WaveEquation::computeSquaredL2Norm() const
{
    double l2_norm = 0.0;
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            l2_norm +=
                u[(i + 1) * (N + 2) + (j + 1)] * u[(i + 1) * (N + 2) + (j + 1)];
        }
    }
    MPI_Reduce((rank == 0) ? MPI_IN_PLACE : &l2_norm,
               &l2_norm,
               1,
               MPI_DOUBLE,
               MPI_SUM,
               0,
               cart_comm);
    return h * l2_norm;
}

void WaveEquation::applyStencil(size_t i, size_t j)
{
    size_t center = i * (N + 2) + j;
    u_new[center] =
        2.0 * u[center] - u_old[center] +
        c_aug * (u[(i + 1) * (N + 2) + j] + u[(i - 1) * (N + 2) + j] +
                 u[i * (N + 2) + (j + 1)] + u[i * (N + 2) + (j - 1)] -
                 4.0 * u[center]);
}

void WaveEquation::saveGrid(size_t timestep) const
{
    std::stringstream ss;
    ss << "./output/wave_" << std::setfill('0') << std::setw(4) << timestep
       << ".bin";
    std::string fname = ss.str();

    // Create derived datatype for interior grid (output grid)
    MPI_Datatype grid;
    const int start[2] = {1, 1};
    const int arrsize[2] = {N + 2, N + 2};
    const int gridsize[2] = {N, N};

    MPI_Type_create_subarray(
        2, arrsize, gridsize, start, MPI_ORDER_C, MPI_DOUBLE, &grid);
    MPI_Type_commit(&grid);

    // Create derived type for file view
    MPI_Datatype view;
    const int startV[2] = {coords[0] * N, coords[1] * N};
    const int arrsizeV[2] = {nums[0] * N, nums[1] * N};
    const int gridsizeV[2] = {N, N};

    MPI_Type_create_subarray(
        2, arrsizeV, gridsizeV, startV, MPI_ORDER_C, MPI_DOUBLE, &view);
    MPI_Type_commit(&view);

    MPI_File fh;

    MPI_File_open(cart_comm,
                  fname.c_str(),
                  MPI_MODE_CREATE | MPI_MODE_WRONLY,
                  MPI_INFO_NULL,
                  &fh);

    MPI_File_set_view(fh, 0, MPI_DOUBLE, view, "native", MPI_INFO_NULL);
    MPI_File_write_all(fh, u.data(), 1, grid, MPI_STATUS_IGNORE);
    MPI_File_close(&fh);
}

WaveEquation::~WaveEquation()
{
    // TODO Question a)
    // Free the Cartesian communicator
}
