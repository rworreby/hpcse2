// File       : memory_copy.cu
// Created    : Sat May 01 2021 12:19:13 PM (+0200)
// Author     : Fabian Wermelinger
// Description: Perform memory copy measurements
// Copyright 2021 ETH Zurich. All Rights Reserved.

#include "CUDATimer.cuh"
#include "utils.h"
#include <iostream>

enum transferDirection {
    H2D, // host to device
    D2H, // device to host
    D2D  // device to device
};

enum allocType {
    pageable,   // pageable memory allocation on host
    page_locked // page-locked memory allocation on host
};

/**
 * @brief Perform memory transfer
 * @param byte number of bytes to transfer
 * @param dir direction of transfer (see transferDirection above)
 * @param method what method to use for host memory allocation (see allocType)
 * @param reps number of repetitions to perform the transfer
 * @return elapsed time for transfer
 */
float transfer(const size_t byte,
               const transferDirection dir,
               const allocType method = page_locked,
               const size_t reps = 10)
{
    // TODO: Implement memory management

    // run measurement
    GPUtimer timer;
    timer.start();
    for (size_t i = 0; i < reps; ++i) {
        // TODO: Perform the memory copies inside this repetitive loop for an
        // average measurement
    }
    timer.stop();

    // TODO: Implement memory management

    // return time in seconds
    return timer.elapsed() / 1000.0f / reps;
}

int main(const int argc, char *[])
{
    bool pinned = false;
    if (2 == argc) {
        // if any argument passed, allocate page-locked memory
        pinned = true;
    }

    // write header
    std::cout << "# Host allocation: " << (pinned ? "page-locked" : "pageable")
              << '\n';
    std::cout << "# MB\tH2D [MB/s]\tD2H [MB/s]\tD2D [MB/s]\n";

    // run measurements
    const size_t max_byte = 1 << 28; // up to 256 MB
    float tH2D, tD2H, tD2D;
    for (size_t i = 4; i <= max_byte; i *= 2) {
        tH2D = transfer(i, H2D, (pinned ? page_locked : pageable));
        tD2H = transfer(i, D2H, (pinned ? page_locked : pageable));
        tD2D = transfer(i, D2D);

        // write results
        const float MB = static_cast<float>(i) / 1024 / 1024;
        std::cout << MB << '\t' << MB / tH2D << '\t' << MB / tD2H << '\t'
                  << 2.0f * MB / tD2D << '\n';
        // factor 2.0 because of DDR
    }

    return 0;
}
