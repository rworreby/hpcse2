#include <mpi.h>
#include <vector>

#define L 1.0

class WaveEquation
{
public:
    ~WaveEquation();

    WaveEquation(size_t a_N,
                 size_t a_procs_per_dim,
                 double a_t_end,
                 MPI_Comm a_comm);
    double initialCondition(double x0, double x1);

    void run();

    void initializeGrid();
    double computeSquaredL2Norm() const;
    void applyStencil(size_t i, size_t j);

    void saveGrid(size_t timestep) const;

private:
    size_t N;              // grid points per direction for this rank
    double h;              // grid spacing (dx = dy = dz = h)
    double dt;             // timestep
    double t;              // current time
    double c_aug;          // augmented wavespeed term
    const double t_end;    // Total simulation time
    double origin[3];      // Spatial origin on local grid
    int coords[3];         // Coordinates withing the cartesian grid
    int nums[2];           // nprocs along each dimension
    std::vector<double> u; // solution vector
    std::vector<double> u_old;
    std::vector<double> u_new;

    int Ntot;
    int procs_per_dim;

    int size; // total number of MPI ranks = procs_per_dim*procs_per_dim
    int rank;

    int rank_plus[2]; // id of neighboring ranks
    int rank_minus[2];

    MPI_Comm cart_comm; // Cartesian topology distributed over the grid
};
