#!/bin/bash

# VUT FIT MITAI
# PRL 2019/2020
# Project 2: Odd-even transposition sort algorithm
# Author: Vladimir Dusek
# Login: xdusek27

# Test correct program execution
if [ $# -ne 1 ] ; then
    printf "Error: invalid program execution\ntry ./test.sh NUMBER\n" >& 2
    exit 1
fi

# Number of random numbers
numbers=$1

# Generate random numbers
dd if=/dev/random bs=1 count="$numbers" of=numbers 2> /dev/null

# Compile
mpic++ --prefix /usr/local/share/OpenMPI ots.cpp -o ots

# Run
mpirun --prefix /usr/local/share/OpenMPI -np "$numbers" ots

# Clean
rm -f numbers ots
