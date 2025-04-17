# Parallel N-Body Gravitational Simulation

## Description
This project implements a **parallelized N-body gravitational simulator** in C++ using OpenMP. The simulation computes the gravitational interactions between particles and updates their positions and velocities over time. It supports multiple initialization modes and output formats compatible with visualization tools.

## Features
- **Parallel Force Computation**: Uses OpenMP to accelerate force calculations between particles
- **Solar System Model**: Includes a built-in 10-body solar system configuration
- **Random Initialization**: Supports generating a user-defined number of particles with randomized positions and velocities
- **File Input/Output**: Accepts `.tsv` files for loading and saving particle states
- **Benchmark Support**: Benchmark script included for testing performance with different particle counts and parameters

## Requirements
- C++11 or later
- g++ compiler with OpenMP support
- Python 3 (optional, for visualization)

## Compilation
To compile both versions (serial and parallel), run:

```bash
make
```
To compile only the parallel version:
```bash
make nbody_parallel
```
## Usage
```bash
./nbody_parallel <input> <dt> <nb_steps> <print_every>
```
## Input Options:
```bash
A number (e.g., 100) → random initialization with N particles

planet → initializes solar system model

A file path (e.g., solar.tsv) → loads particle data from TSV file
```
Example Runs:
**`./nbody_parallel planet 200 5000 500`**
**`./nbody_parallel 100 1 10000 100`**

## Cleaning up
To remove the compiled executable and object files:
**`make clean`**

## Benchmarking
To run the pre-configured benchmark on Centaurus:
**`sbatch benchmark.sh`**
