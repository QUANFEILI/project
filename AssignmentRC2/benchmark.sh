#!/bin/bash

#SBATCH --job-name=bfs_compare_parallel
#SBATCH --partition=Centaurus
#SBATCH --ntasks=1
#SBATCH --time=01:00:00
#SBATCH --mem=10G

echo "Compiling sequential BFS program..."
g++ -o client client.cpp -lcurl -pthread -std=c++17

echo "Compiling parallel BFS program..."
g++ -o client_parallel client_parallel.cpp -lcurl -pthread -std=c++17
if [ $? -ne 0 ]; then
    echo "Parallel version compilation failed!"
    exit 1
fi

echo "Running sequential BFS: Tom Hanks (Depth 2)..."
/usr/bin/time -f "Execution time: %e seconds" ./client "Tom Hanks" 2 > seq_results_tom_hanks.txt 2> seq_time_tom_hanks.txt

echo "Running parallel BFS: Tom Hanks (Depth 2)..."
/usr/bin/time -f "Execution time: %e seconds" ./client_parallel "Tom Hanks" 2 > par_results_tom_hanks.txt 2> par_time_tom_hanks.txt

echo "Running sequential BFS: Matt Damon (Depth 2)..."
/usr/bin/time -f "Execution time: %e seconds" ./client "Matt Damon" 2 > seq_results_matt_damon.txt 2> seq_time_matt_damon.txt

echo "Running parallel BFS: Matt Damon (Depth 2)..."
/usr/bin/time -f "Execution time: %e seconds" ./client_parallel "Matt Damon" 2 > par_results_matt_damon.txt 2> par_time_matt_damon.txt

echo "Running sequential BFS: Tom Hanks (Depth 3)..."
/usr/bin/time -f "Execution time: %e seconds" ./client "Tom Hanks" 3 > seq_results_tom_hanks_depth3.txt 2> seq_time_tom_hanks_depth3.txt

echo "Running parallel BFS: Tom Hanks (Depth 3)..."
/usr/bin/time -f "Execution time: %e seconds" ./client_parallel "Tom Hanks" 3 > par_results_tom_hanks_depth3.txt 2> par_time_tom_hanks_depth3.txt

echo "All BFS comparison tasks completed!"
