# File       : meson.build
# Created    : Thu Apr 15 2021 09:48:56 AM (+0200)
# Description: Build definitions for HW4. See https://mesonbuild.com/Manual.html
# Copyright 2021 ETH Zurich. All Rights Reserved.

# project declaration must always be the first that you do
project('Homework4', ['cpp'],
  version: '1.0.0',
  default_options: [
    'cpp_std=c++11',
    'b_ndebug=if-release',
    'warning_level=3',
  ]
)

# these are the include directories that we need in out code
inc = include_directories('include')

# our code will (weakly) depend on OpenMP and MPI. If these dependencies are not
# found code that requires them will not be built
openmp_dep = dependency('openmp', required: false)
mpi_dep = dependency('mpi', language: 'cpp', required: false)

# here we define an executable that should be built 
# (see: https://mesonbuild.com/Reference-manual.html#executable)

executable('main', # the name of the executable
  files(['main.cpp', 'src/LaplacianSmoother.cpp']), # the required sources to build it
  include_directories: inc, # we need to specify where the headers are located
  dependencies: [openmp_dep], # the dependencies it needs
  install: false # whether we want to install it (we do not need to install it
                 # for this exercise, you may want to install your real project)
)

if mpi_dep.found()
  # TODO: Define an executable for your MPI code
  # executable('mainMPI', ...)
endif
