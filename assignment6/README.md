# CUDA N-Body Simulation

## Description
This project implements a parallel N-body simulation using CUDA C++. It calculates gravitational forces between particles and updates their positions and velocities using Newtonian mechanics, fully offloaded to the GPU for acceleration.

It is based on the sequential **nbody.cpp** provided in the assignment, and extended to use GPU parallelism via CUDA kernels. The code is compatible with Centaurus GPU nodes.

## Features
- **CUDA Kernels**: GPU-parallelized computation of forces and position updates
- **Memory Management**: xplicit use of cudaMalloc and cudaMemcpy for data movement
- **Solar System & Random Modes**: Supports preset planetary setup or large-scale particle clouds
- **Sequential Version**: Includes a baseline BFS implementation for comparison
- **Benchmark Script**: Pre-configured SLURM script for performance testing on Centaurus

## Requirements
- CUDA 12.4 (on Centaurus: **module load cuda/12.4**) **this is important because need to use GPU**
- C++17 or later
- `g++` compiler
- Linux environment (Tested on Centaurus cluster)

## Compilation
To compile both versions:
```bash
make
```
-This will generate:
    **nbody_gpu** → GPU-accelerated simulation executable
    
## Usage
```bash
./nbody_gpu <input> <dt> <nbsteps> <printevery> <blocksize>
```
Example:
**`./nbody_gpu 1000 0.01 100 10 256`**

## Cleaning up
To remove the compiled executable and object files:
**`make clean`**

## Benchmarking
To run the pre-configured benchmark on Centaurus:
**`sbatch benchmark.sh`**