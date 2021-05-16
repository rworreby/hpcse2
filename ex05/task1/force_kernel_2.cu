#include <cuda_runtime.h>
#include <cstdio>

// TODO Improve this kernel.
//      Create a new file for each new category of optimizations you
//      successively apply {force_kernel_2.cu, force_kernel_3.cu, ...}
__global__ void computeForcesKernel(int N, const double3 *p, double3 *f) {

    for(int idx = blockIdx.x * blockDim.x + threadIdx.x;
            idx < N;
            idx += gridDim.x * blockDim.x){

        double f_temp_x = 0.0;
        double f_temp_y = 0.0;
        double f_temp_z = 0.0;
        for (int i = 0; i < N; ++i) {
            if(i != idx){
                double dx = p[i].x - p[idx].x;
                double dy = p[i].y - p[idx].y;
                double dz = p[i].z - p[idx].z;
                double r = sqrt(dx * dx + dy * dy + dz * dz);
                double inv_r = 1.0 / r;
                double inv_r_3 = inv_r * inv_r * inv_r;

                f_temp_x += dx * inv_r_3;
                f_temp_y += dy * inv_r_3;
                f_temp_z += dz * inv_r_3;
            }
        }
        f[idx].x = f_temp_x;
        f[idx].y = f_temp_y;
        f[idx].z = f_temp_z;
    }
}

void computeForces(int N, const double3 *p, double3 *f) {
    constexpr int numThreads = 1024;
    int numBlocks = (N + numThreads - 1) / numThreads;
    computeForcesKernel<<<numBlocks, numThreads>>>(N, p, f);
}
