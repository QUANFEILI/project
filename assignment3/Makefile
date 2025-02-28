CXX = g++
CXXFLAGS = -std=c++11 -lcurl -I/path/to/rapidjson/include

all: bfs

bfs: bfs.cpp
	$(CXX) $(CXXFLAGS) -o bfs bfs.cpp

clean:
	rm -f bfs