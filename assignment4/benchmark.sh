#!/bin/bash
#SBATCH --job-name=bfs_test
#SBATCH --partition=Centaurus
#SBATCH --time=00:10:00
#SBATCH --mem=2G

make || exit 1

# Test parallel version：Tom Hanks Depth3
./par_level_client "Tom Hanks" 3 > par_tom_hanks_3_results.txt 2> par_tom_hanks_3_time.txt

# Test parallel version：Tom Hanks Depth2
./par_level_client "Tom Hanks" 2 > par_tom_hanks_2_results.txt 2> par_tom_hanks_2_time.txt

# Test serial version：Tom Hanks Depth2
./level_client "Tom Hanks" 2 > seq_tom_hanks_2_results.txt 2> seq_tom_hanks_2_time.txt

echo "All BFS tasks completed!"