nbody: nbody.cpp
	g++ -O3 nbody.cpp -o nbody

nbody_parallel: nbody_parallel.cpp
	g++ -O3 -fopenmp -std=c++11 nbody_parallel.cpp -o nbody_parallel

solar.out: nbody
	date
	./nbody planet 200 5000000 10000 > solar.out
	date

random.out: nbody
	date
	./nbody 1000 1 10000 100 > random.out
	date

solar.pdf: solar.out
	python3 plot.py solar.out solar.pdf 1000

clean:
	rm -f nbody nbody_parallel *.out *.txt *.tsv *.pdf
