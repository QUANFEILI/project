#!/bin/bash

#SBATCH --job-name=nbody_benchmark
#SBATCH --partition=Centaurus
#SBATCH --ntasks=1
#SBATCH --time=02:00:00
#SBATCH --mem=20G


g++ -o nbody nbody.cpp

# Running solar system simulation
echo "Running solar system simulation..."
/usr/bin/time -f "Time: %e seconds" ./nbody 9 200 5000000 1000 solar_output.tsv > solar_results.txt

# Run 100 particle simulations
echo "Running 100 particles simulation..."
/usr/bin/time -f "Time: %e seconds" ./nbody 100 1 10000 100 output_100.tsv > results_100.txt

# Run 1000 particle simulations
echo "Running 1000 particles simulation..."
/usr/bin/time -f "Time: %e seconds" ./nbody 1000 1 10000 100 output_1000.tsv > results_1000.txt

echo "All simulations completed!"
