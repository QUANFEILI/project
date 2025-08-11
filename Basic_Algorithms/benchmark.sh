#!/bin/bash

#SBATCH --job-name=mergesort_benchmark
#SBATCH --partition=Centaurus
#SBATCH --ntasks=1
#SBATCH --time=00:10:00

output_file="results.txt"


g++ -o mergesort mergesort.cpp

for size in 10 100 1000 10000 100000 1000000 10000000 100000000 1000000000
do
    ./mergesort $size >> $output_file
done
