#!/bin/bash
source /cluster/apps/local/env2lmod.sh
module purge
module load gcc/8.2.0 python/3.6.4 cmake/3.9.4 openmpi/4.0.2
export MPICXX=`which mpicxx`
