#!/bin/bash

#SBATCH --job-name=mergesort
#SBATCH --partition=Centaurus
#SBATCH --ntasks=1
#SBATCH --time=00:30:00
#SBATCH --mem=10G

echo "Compiling programs..."
make
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

# Sequential version testing
echo "Running sequential merge sort (10,000)..."
/usr/bin/time -f "Execution time: %e seconds" ./mergesort_seq 10000 > result_seq_10k.txt 2> time_seq_10k.txt

echo "Running sequential merge sort (100,000)..."
/usr/bin/time -f "Execution time: %e seconds" ./mergesort_seq 100000 > result_seq_100k.txt 2> time_seq_100k.txt

echo "Running sequential merge sort (1,000,000)..."
/usr/bin/time -f "Execution time: %e seconds" ./mergesort_seq 1000000 > result_seq_1M.txt 2> time_seq_1M.txt

# Parallel version testing
echo "Running parallel merge sort (10,000)..."
/usr/bin/time -f "Execution time: %e seconds" ./mergesort_parallel 10000 > result_par_10k.txt 2> time_par_10k.txt

echo "Running parallel merge sort (100,000)..."
/usr/bin/time -f "Execution time: %e seconds" ./mergesort_parallel 100000 > result_par_100k.txt 2> time_par_100k.txt

echo "Running parallel merge sort (1,000,000)..."
/usr/bin/time -f "Execution time: %e seconds" ./mergesort_parallel 1000000 > result_par_1M.txt 2> time_par_1M.txt

echo "All merge sort benchmarks completed!"
