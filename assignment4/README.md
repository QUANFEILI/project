# Parallel BFS Graph Traversal with Web API

## Description
This project implements a **parallel** Breadth-First Search (BFS) algorithm in C++ to traverse a graph accessed via a web API. The program uses multi-threading to concurrently expand nodes at each level, significantly improving traversal speed compared to the sequential version.

## Features
- **Parallel BFS Traversal**: Implements multi-threaded BFS with level-by-level node expansion
- **Web API Integration**: Dynamically fetches neighboring nodes from `hollywood-graph-crawler` server
- **Thread Safety**: Uses mutex locks to protect shared resources
- **Performance Comparison**: Includes both sequential (`level_client`) and parallel (`par_level_client`) versions for benchmarking

## Requirements
- C++11 or later
- `g++` compiler
- Linux environment (Tested on Centaurus cluster)
- libcurl: `sudo apt-get install libcurl4-openssl-dev`
- RapidJSON: Included in repository

## Compilation
Compile both versions using:
**`make`**

## Usage
To run the BFS program::
```bash
./par_level_client <start_node> <depth>
```
Example:
**`./par_level_client "Tom Hanks" 3`**

Sequential Version (for comparison):
**`./level_client <start_node> <depth>`**

## Cleaning up
To remove the compiled executable and object files:
**`make clean`**

## Benchmarking
To run the pre-configured benchmark on Centaurus:
**`sbatch benchmark.sh`**