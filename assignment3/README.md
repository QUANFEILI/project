# BFS Graph Traversal with Web API

## Description
This project implements a Breadth-First Search (BFS) algorithm in C++ to traverse a graph accessed via a web API. The program takes a starting node and a traversal depth as input, queries the server for neighboring nodes, and returns all nodes reachable within the given depth.

## Features
- BFS Traversal: Implements a breadth-first search algorithm to explore the graph.
- Web API Integration: Dynamically fetches neighboring nodes from a web-based graph server.
- JSON Parsing: Uses RapidJSON to parse API responses.
- Performance Testing: Measures execution time for different starting nodes and traversal depths.

## Requirements
- C++17 or later
- `g++` compiler
- Linux or macOS environment (Centaurus cluster supported)
- libcurl: Library for making HTTP requests.
- RapidJSON: Header-only JSON parser for C++.

## Compilation
Run the following command to compile the program:
**`g++ -o bfs bfs.cpp -I ./rapidjson/include -lcurl`**

## Usage
To run the BFS program::
```bash
./bfs <start_node> <depth>
```
Example:
Run BFS with Tom Hanks as the starting node and a depth of 2:
**`./bfs "Tom Hanks" 2`**

Run BFS with Matt Damon as the starting node and a depth of 3:
**`./bfs "Matt Damon" 3`**

## Cleaning up
To remove the compiled executable and object files:
**`make clean`**

## Benchmarking
To run the pre-configured benchmark on Centaurus:
**`sbatch benchmark.sh`**

