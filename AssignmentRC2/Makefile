CXX = g++
CXXFLAGS = -std=c++17 -O2
LDFLAGS = -lcurl -pthread

all: client client_parallel

client: client.cpp
	$(CXX) $(CXXFLAGS) -I rapidjson/include client.cpp -o client $(LDFLAGS)

client_parallel: client_parallel.cpp
	$(CXX) $(CXXFLAGS) -I rapidjson/include client_parallel.cpp -o client_parallel $(LDFLAGS)

clean:
	rm -f client client_parallel
