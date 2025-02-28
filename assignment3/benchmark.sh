#!/bin/bash


#SBATCH --job-name=bfs_benchmark       
#SBATCH --partition=Centaurus          
#SBATCH --ntasks=1                     
#SBATCH --time=02:00:00                
#SBATCH --mem=20G                  
#SBATCH --output=bfs_benchmark_%j.out 
#SBATCH --error=bfs_benchmark_%j.err  



echo "Compiling BFS program..."
g++ -std=c++11 -I/home/vboxuser/rapidjson/include bfs.cpp -o bfs -lcurl
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi


output_file="benchmark_results.txt"


> "$output_file"


start_nodes=("Tom Hanks" "Brad Pitt" "Leonardo DiCaprio")
depths=(1 2 3)

for node in "${start_nodes[@]}"; do
    for depth in "${depths[@]}"; do
        echo "Running BFS with start node: $node, depth: $depth"

        start_time=$(date +%s%N)
        ./bfs "$node" "$depth"
        end_time=$(date +%s%N)

        execution_time=$(( (end_time - start_time) / 1000000 ))

        echo "Start Node: $node, Depth: $depth, Execution Time: ${execution_time} ms" >> "$output_file"
        echo "----------------------------------------" >> "$output_file"
    done
done

echo "Benchmarking completed. Results saved to $output_file."