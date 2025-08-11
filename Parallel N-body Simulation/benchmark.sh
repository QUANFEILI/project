#!/bin/bash

#SBATCH --job-name=nbody_benchmark
#SBATCH --partition=GPU
#SBATCH --ntasks=1
#SBATCH --time=00:30:00
#SBATCH --mem=8G
#SBATCH --gres=gpu:1

module load cuda/12.4

echo "Compiling CUDA N-body simulation..."
nvcc -O3 -arch=sm_61 -o nbody_gpu nbody_gpu.cu
if [ $? -ne 0 ]; then
    echo "CUDA compilation failed!"
    exit 1
fi

echo "Running benchmark: 1000 particles, 10 steps..."
/usr/bin/time -f "Execution time: %e seconds" ./nbody_gpu 1000 0.01 10 10 256 > nbody_1000.txt 2> time_1000.txt

echo "Running benchmark: 10000 particles, 10 steps..."
/usr/bin/time -f "Execution time: %e seconds" ./nbody_gpu 10000 0.01 10 10 256 > nbody_10000.txt 2> time_10000.txt

echo "Running benchmark: 100000 particles, 5 steps..."
/usr/bin/time -f "Execution time: %e seconds" ./nbody_gpu 100000 0.01 5 5 256 > nbody_100000.txt 2> time_100000.txt

echo "All CUDA benchmarks completed!"
