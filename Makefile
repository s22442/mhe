DIST = if [ ! -d dist ]; then mkdir dist; fi
CC = g++
CFLAGS = -std=c++17

all: dist/genetic-algorithm dist/hill-climbing simulated-annealing tabu-search

dist/GarbageBag.o: src/GarbageBag.cpp
	$(DIST); $(CC) -c -o dist/GarbageBag.o src/GarbageBag.cpp $(CFLAGS)

dist/Solution.o: src/Solution.cpp
	$(DIST); $(CC) -c -o dist/Solution.o src/Solution.cpp $(CFLAGS)

dist/utils.o: src/utils.cpp
	$(DIST); $(CC) -c -o dist/utils.o src/utils.cpp $(CFLAGS)

dist/genetic-algorithm.o: src/genetic-algorithm/main.cpp
	$(DIST); $(CC) -c -o dist/genetic-algorithm.o src/genetic-algorithm/main.cpp $(CFLAGS)

dist/genetic-algorithm: dist/genetic-algorithm.o dist/utils.o dist/GarbageBag.o dist/Solution.o
	$(DIST); $(CC) -o dist/genetic-algorithm dist/genetic-algorithm.o dist/utils.o dist/GarbageBag.o dist/Solution.o $(CFLAGS)

dist/hill-climbing.o: src/hill-climbing/main.cpp
	$(DIST); $(CC) -c -o dist/hill-climbing.o src/hill-climbing/main.cpp $(CFLAGS)

dist/hill-climbing: dist/hill-climbing.o dist/utils.o dist/GarbageBag.o dist/Solution.o
	$(DIST); $(CC) -o dist/hill-climbing dist/hill-climbing.o dist/utils.o dist/GarbageBag.o dist/Solution.o $(CFLAGS)

dist/simulated-annealing.o: src/simulated-annealing/main.cpp
	$(DIST); $(CC) -c -o dist/simulated-annealing.o src/simulated-annealing/main.cpp $(CFLAGS)

dist/simulated-annealing: dist/simulated-annealing.o dist/utils.o dist/GarbageBag.o dist/Solution.o
	$(DIST); $(CC) -o dist/simulated-annealing dist/simulated-annealing.o dist/utils.o dist/GarbageBag.o dist/Solution.o $(CFLAGS)

dist/tabu-search.o: src/tabu-search/main.cpp
	$(DIST); $(CC) -c -o dist/tabu-search.o src/tabu-search/main.cpp $(CFLAGS)

dist/tabu-search: dist/tabu-search.o dist/utils.o dist/GarbageBag.o dist/Solution.o
	$(DIST); $(CC) -o dist/tabu-search dist/tabu-search.o dist/utils.o dist/GarbageBag.o dist/Solution.o $(CFLAGS)

clean:
	rm -rf dist && mkdir dist
