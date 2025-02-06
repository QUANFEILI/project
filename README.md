# Merge Sort Benchmark on Centaurus

## Overview
This project implements the Merge Sort algorithm in C++ and benchmarks its performance with different input sizes. The performance is tested and recorded using the SLURM workload manager on the Centaurus cluster.

## Files in the Project

- **`mergesort.cpp`**: Implements the Merge Sort algorithm.
- **`benchmark.sh`**: Automates the compilation and execution of the program with various input sizes.
- **`results.txt`**: Stores the output of the benchmark tests.
- **`plot_benchmark.py`**: Reads the results file and generates a log-log plot of the performance.
- **`benchmark_plot.png`**: The generated performance plot.

## How to Compile and Run

### 1. Compiling the Program
To compile the program locally:
g++ -O2 -Wall -o mergesort mergesort.cpp

### 2. Running the Program Locally
Execute the program by specifying the size of the array:
./mergesort <size_of_array>
For example: 
./mergesort 100000

### 3. Running on Centaurus Cluster
Use the provided benchmark.sh script to submit a SLURM job:
sbatch benchmark.sh

### 4. Generating the Performance Plot
I am currently using a VSC extensions called "Matplotlib Pilot". After installing the extension, it can generate code that can generate graphs according to the tutorials in the extension page.

### Benchmark Results and Analysis
The program follows an expected time complexity of O(n log n) for Merge Sort.
Results:
10, 0 ms
100, 0 ms
1000, 0 ms
10000, 1 ms
100000, 13 ms
1000000, 157 ms
10000000, 1655 ms
100000000, 17676 ms

Analysis: The results indicate that smaller input sizes (such as 10, 100, 1000) can almost be completed immediately (0ms), as expected. For larger input sizes, the execution time roughly increases with the increase of O (n log n) complexity.
However, if the input size reaches 10^9 (1000000000), an OOM (Out of Memory) error may be encountered on the system(check slurm-6881.out)


