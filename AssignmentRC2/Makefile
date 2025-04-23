#CXXFLAGS=-I ~/prgs/rapidjson/include
LDFLAGS=-lcurl
LD=g++
CC=g++

all: client

client: client.o
	$(LD) $< -o $@ $(LDFLAGS)

clean:
	-rm client client.o



CXX = g++
CXXFLAGS = -std=c++17 -O2
LDFLAGS = -lcurl -pthread

# Default build rule
all: client client_parallel

# Sequential version (original)
client: client.cpp
	$(CXX) $(CXXFLAGS) client.cpp -o client $(LDFLAGS)

# Parallel version (with multithreading)
client_parallel: client_parallel.cpp
	$(CXX) $(CXXFLAGS) client_parallel.cpp -o client_parallel $(LDFLAGS)

# Clean build artifacts
clean:
	rm -f client client_parallel
