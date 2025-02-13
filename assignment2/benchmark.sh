#!/bin/bash

#SBATCH --job-name=nbody_benchmark
#SBATCH --partition=Centaurus
#SBATCH --ntasks=1
#SBATCH --time=02:00:00
#SBATCH --mem=20G

output_file="nbody_results.txt"

g++ -o nbody nbody.cpp

for config in "9 200 5000000 1000 solar_output.tsv" "100 1 10000 100 output_100.tsv" "1000 1 10000 100 output_1000.tsv"
do
    ./nbody $config >> $output_file
done
