#include <cassert>
#include <iostream>
#include <mpi.h>
#include <unistd.h>
#include <vector>

//
// Data Structures
//

struct particle {
    float x;
    float y;
    float z;
    float mass;
};

struct system_positions {
    std::vector<float> x;
    std::vector<float> y;
    std::vector<float> z;
};

//
// Helper Functions (unimportant)
//

void fill_particles(std::vector<particle> &system)
{
    for (unsigned int i = 0; i < system.size(); ++i) {
        system[i].x = i * 0.1;
        system[i].y = i * 0.5;
        system[i].z = i * 0.8;
        system[i].mass = i * i * 0.4;
    }
}

void print(std::vector<particle> const &system)
{
    for (unsigned int i = 0; i < system.size(); ++i)
        std::cout << system[i].x << ", " << system[i].y << ", " << system[i].z
                  << "\t mass: " << system[i].mass << "\n";
}

void print(system_positions const &system)
{
    assert(system.x.size() == system.y.size());
    assert(system.x.size() == system.z.size());
    for (unsigned int i = 0; i < system.x.size(); ++i)
        std::cout << system.x[i] << ", " << system.y[i] << ", " << system.z[i]
                  << "\n";
}

//
// The MPI Program
//

int main(int argc, char **argv)
{
    ////////////////   Init MPI    ////////////////

    unsigned int n = 10;
    MPI_Init(&argc, &argv);
    int comm_size;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    if (comm_size != 2) {
        std::cerr << "ERROR: Needs to run with 2 MPI processes." << std::endl;
        MPI_Finalize();
        return -1;
    }
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    ////////////////  Question a)  ////////////////
    if (rank == 0)
        std::cout << std::endl
                  << std::endl
                  << "Beginning of Question a)" << std::endl;

    // Use an array(vector) of structures (AoS) to store the data
    std::vector<particle> system(n);
    if (rank == 0) {
        fill_particles(system);

        // Print data to be sent
        std::cout << "Rank 0\n";
        print(system);
        std::cout << std::endl;
    }

    //
    // TODO:
    // * Create an MPI datatype for all the position parts of 'system'
    // * Send all positions from rank 0 to rank 1
    // Do not send any masses
    //
    // implement code here: define aos_particle_t

    if (rank == 1) {
        // Print received data
        std::cout << "Rank 1\n";
        print(system);
        std::cout << std::endl;
    }

    ////////////////  Question b)  ////////////////
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0)
        std::cout << std::endl
                  << std::endl
                  << "Beginning of Question b)" << std::endl;

    // Use a structure of arrays(vectors) (SoA) to store the positions
    system_positions positions;
    positions.x.resize(n);
    positions.y.resize(n);
    positions.z.resize(n);

    //
    // Note the different data layout in this task!
    //
    // TODO:
    // * Create an MPI datatype for the positions in SoA format
    // * Send system from rank 0 to rank 1 (same send as the previous task)
    // * Receive the data in the `positions` variable on rank 1
    //
    // implement code here: define soa_particle_t

    // Print received data
    if (rank == 1) {
        // Print received data
        print(positions);
        std::cout << std::endl;
    }

    ////////////////   Cleanup    ////////////////
    MPI_Type_free(&aos_particle_t);
    MPI_Type_free(&soa_particle_t);

    MPI_Finalize();
    return 0;
}
