#!/bin/bash

#SBATCH --job-name=nbody_parallel
#SBATCH --partition=Centaurus
#SBATCH --ntasks=1
#SBATCH --time=01:00:00
#SBATCH --mem=10G

echo "Compiling programs..."
make
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

export OMP_NUM_THREADS=4

echo "Running: Solar System (dt=200, steps=5000000)..."
/usr/bin/time -f "Execution time: %e seconds" ./nbody_parallel planet 200 5000000 100000 > results_solar.tsv 2> time_solar.txt

echo "Running: 100 particles (dt=1, steps=10000)..."
/usr/bin/time -f "Execution time: %e seconds" ./nbody_parallel 100 1 10000 1000 > results_100.tsv 2> time_100.txt

echo "Running: 1000 particles (dt=1, steps=10000)..."
/usr/bin/time -f "Execution time: %e seconds" ./nbody_parallel 1000 1 10000 1000 > results_1000.tsv 2> time_1000.txt

echo "All benchmark tasks completed!"
