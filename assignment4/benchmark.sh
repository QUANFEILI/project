#!/bin/bash

#SBATCH --job-name=bfs_graph_traversal
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

echo "Running Parallel BFS: Tom Hanks (Depth 3)..."
/usr/bin/time -f "Execution time: %e seconds" ./par_level_client "Tom Hanks" 3 > par_results_tom_hanks_3.txt 2> par_time_tom_hanks_3.txt

echo "Running Parallel BFS: Tom Hanks (Depth 2)..."
/usr/bin/time -f "Execution time: %e seconds" ./par_level_client "Tom Hanks" 2 > par_results_tom_hanks_2.txt 2> par_time_tom_hanks_2.txt

echo "Running Sequential BFS: Tom Hanks (Depth 2)..."
/usr/bin/time -f "Execution time: %e seconds" ./level_client "Tom Hanks" 2 > seq_results_tom_hanks_2.txt 2> seq_time_tom_hanks_2.txt

echo "All tasks completed!"