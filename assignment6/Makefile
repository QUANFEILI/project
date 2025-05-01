# === Makefile for CUDA-based N-Body Assignment ===

NVCC = nvcc
NVCC_FLAGS = -O3 -arch=sm_61
TARGET = nbody_gpu
SRC = nbody_gpu.cpp

# Default target
all: $(TARGET)

# Compile CUDA program
$(TARGET): $(SRC)
	$(NVCC) $(NVCC_FLAGS) -o $(TARGET) $(SRC)

# Example: Run solar system test and generate output
solar.out: $(TARGET)
	date
	./$(TARGET) planet 200 5000000 10000 256 > solar.out
	date

solar.pdf: solar.out
	python3 plot.py solar.out solar.pdf 1000

# Example: Run random test case (1000 particles)
random.out: $(TARGET)
	date
	./$(TARGET) 1000 1 10000 100 256 > random.out
	date

# Clean everything
clean:
	rm -f $(TARGET) *.out *.pdf output.log error.log
