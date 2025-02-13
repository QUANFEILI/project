# N-Body Simulation

## Description
This project simulates an N-body gravitational system using numerical integration methods. The simulation computes gravitational forces between particles and updates their positions and velocities over time.

## Features
- Random initialization of particles
- Supports loading initial conditions from a file (`solar.tsv`)
- Saves simulation state at specified intervals
- Measures execution time

## Requirements
- C++17 or later
- `g++` compiler
- Linux or macOS environment (Centaurus cluster supported)

## Compilation
Run the following command to compile the program:
**`make`**

## Usage
To run a simulation with randomly initialized particles:
```bash
./nbody <num_particles> <dt> <steps> <output_interval> <output_file>
```
Example:
**`./nbody 100 1 10000 100 output_100.tsv`**

To load initial conditions from `solar.tsv` (by passing `0` as the number of particles):
**`./nbody 0 200 5000000 1000 solar_output.tsv`**

## Cleaning up
To remove the compiled executable and object files:
**`make clean`**

## Benchmarking
To run the pre-configured benchmark on Centaurus:
**`sbatch benchmark.sh`**

