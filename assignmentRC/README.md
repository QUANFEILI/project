# Parallel Merge Sort in C++

## Description
This project implements a **parallel merge sort** algorithm in C++ using `std::thread`. It compares the performance of sequential and parallel implementations across various array sizes to demonstrate the benefits of multi-threading in divide-and-conquer sorting.

## Features
- **Parallel Merge Sort**: Recursively sorts array halves in parallel when above a size threshold
- **Thread Management**: Dynamically spawns threads based on array segment size
- **Thread Safety**: Uses independent buffers to avoid race conditions
- **Sequential Version**: Includes a standard sequential merge sort implementation
- **Performance Comparison**: Benchmarks both implementations to analyze speedup and optimal thresholds

## Requirements
- C++17 or later
- `g++` compiler
- Linux environment (Tested on Centaurus cluster)

## Compilation
To compile both versions:
```bash
make mergesort_seq
make mergesort_parallel
```

## Usage
Run the programs with the desired array size as argument:
```bash
./mergesort_seq <array_size>
./mergesort_parallel <array_size>
```
Example:
**`./mergesort_parallel 1000000`**

## Cleaning up
To remove the compiled executable and object files:
**`make clean`**

## Benchmarking
To run the pre-configured benchmark on Centaurus:
**`sbatch benchmark.sh`**