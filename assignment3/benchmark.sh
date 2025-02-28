#!/bin/bash

#SBATCH --job-name=bfs_graph_traversal
#SBATCH --partition=Centaurus
#SBATCH --ntasks=1
#SBATCH --time=01:00:00
#SBATCH --mem=10G


echo "Compiling BFS C++ program..."
g++ -o bfs bfs.cpp -I ./rapidjson/include -lcurl
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

echo "Running BFS: Tom Hanks (Depth 2)..."
/usr/bin/time -f "Execution time: %e seconds" ./bfs "Tom Hanks" 2 > bfs_results_tom_hanks.txt 2> bfs_time_tom_hanks.txt

echo "Running BFS: Matt Damon (Depth 2)..."
/usr/bin/time -f "Execution time: %e seconds" ./bfs "Matt Damon" 2 > bfs_results_matt_damon.txt 2> bfs_time_matt_damon.txt

echo "Running BFS: Tom Hanks (Depth 3)..."
/usr/bin/time -f "Execution time: %e seconds" ./bfs "Tom Hanks" 3 > bfs_results_tom_hanks_depth3.txt 2> bfs_time_tom_hanks_depth3.txt

echo "All BFS tasks completed!"
