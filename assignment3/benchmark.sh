#!/bin/bash

#SBATCH --job-name=bfs_graph_traversal
#SBATCH --partition=Centaurus
#SBATCH --ntasks=1
#SBATCH --time=01:00:00
#SBATCH --mem=10G


echo " Compiling BFS C++ program..."
g++ -o bfs graph_crawler.cpp -I /usr/include/x86_64-linux-gnu -I /usr/include/rapidjson -lcurl


# Run BFS for Tom Hanks with depth 2
echo "Running BFS: Tom Hanks (Depth 2)..."
/usr/bin/time -f "Execution time: %e seconds" ./bfs "Tom Hanks" 2 > bfs_results_tom_hanks.txt

# Run BFS for Matt Damon with depth 2
echo "Running BFS: Matt Damon (Depth 2)..."
/usr/bin/time -f "Execution time: %e seconds" ./bfs "Matt Damon" 2 > bfs_results_matt_damon.txt

# Run BFS for Tom Hanks with depth 3 (larger search space)
echo "Running BFS: Tom Hanks (Depth 3)..."
/usr/bin/time -f "Execution time: %e seconds" ./bfs "Tom Hanks" 3 > bfs_results_tom_hanks_depth3.txt

echo "All BFS tasks completed!"
