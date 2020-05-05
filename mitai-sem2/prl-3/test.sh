#!/bin/bash

# VUT FIT MITAI
# PRL 2019/2020
# Project 3: Viditelnost
# Author: Vladimir Dusek
# Login: xdusek27

# Test correct program execution
if [ $# -ne 1 ] ; then
    printf "Error: invalid program execution\ntry ./test.sh STRING\n" >& 2
    exit 1
fi

# Get length of the input
N=$(python -c "
print(len(\"${1}\".split(',')))"
)

# Get number of processors according
# to equation: n/p >= log2(p)
P=$(python -c "
from math import pow, ceil, log
optimal_p = 1
n = pow(2, ceil(log(int(\"${N}\")) / log(2)))
for p in [2, 4, 8, 16, 32]:
    if (2 ** (n / p)) >= p:
        optimal_p = p
    else:
        break
print(optimal_p)"
)

# Compile
mpic++ -O3 --prefix /usr/local/share/OpenMPI vid.cpp -o vid

# Run
mpirun --prefix /usr/local/share/OpenMPI -np "$P" vid "$1" "$N"

# Clean
rm -f vid
