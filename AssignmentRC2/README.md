# Parallel BFS in C++

## Description
This project implements a **parallel breadth-first search (BFS)** algorithm in C++ using `std::thread`. It traverses a graph dynamically via a web API and compares the performance of sequential and parallel implementations. The parallel version uses worker threads to concurrently expand BFS layers, improving throughput when fetching neighbors over the network.

## Features
- **Parallel BFS Traversal**: Uses 8 threads to explore nodes concurrently across BFS levels
- **Web API Integration**: Retrieves neighbor lists via HTTP from a remote graph service
- **Thread Safety**: Synchronizes access to shared queue and visited set using `mutex` and `condition_variable`
- **Sequential Version**: Includes a baseline BFS implementation for comparison
- **Performance Benchmarking**: Automates side-by-side tests using Slurm on Centaurus

## Requirements
- C++17 or later
- `g++` compiler
- Linux environment (Tested on Centaurus cluster)
- `libcurl` (Install with `sudo apt-get install libcurl4-openssl-dev`)
- RapidJSON (included under `AssignmentRC2/rapidjson`)

## Compilation
To compile both versions:
```bash
make
```
-This will generate:
    client for the sequential version
    client_parallel for the multithreaded version
    
## Usage
Run either program with a start node and desired depth:
```bash
./client_parallel <start_node> <depth>
./client <start_node> <depth>

```
Example:
**`./client_parallel "Tom Hanks" 2`**

## Cleaning up
To remove the compiled executable and object files:
**`make clean`**

## Benchmarking
To run the pre-configured benchmark on Centaurus:
**`sbatch benchmark.sh`**