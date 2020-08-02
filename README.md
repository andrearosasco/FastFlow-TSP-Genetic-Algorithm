# FastFlow-TSP-Genetic-Algorithm
This repository contains three implementation of a genetic algorithm solving the travelling salesman problem: a sequential one, one using standard c++ parallel mechanism and one using the parallel pattern library FastFlow. A study of the two implementations performances can be found in the report SPM.pdf.

## How to compile:
To compile one of the implementations is sufficient to enter one of the followin
    -sequential     -selection_farm     -normal_form     -async     -thread
and run "make solve_tsp"

## How to run:
All the implementations except for the sequential can be executed with the following arguments:
    ./solve_tsp [population_size] [generations] [nodes] [nw] [seed]
the sequential version arguments are the same but [nw] (number of workers) is not presetn.

The parallel implementations run nw genetic algorithms each with a population of k/nw chromosomes.
In normal_form and in thread is possible to comment the row "k = k / nw" so that the number of chromosomes for each populations is kept the same.

## Project structure:

#### common
functions used by most of the implementations:
- tsp.cpp - problem definition and fitness score function
- genetic.cpp - genetic algorithm operators (crossover, mutation, ...)
- utils.cpp - common utilities (e.g. time measures, random number generator)

#### sequential
sequential implementation

#### ff_parallel
two implementations with the Fastflow framework
- selection_farm - test implementation used to understand the impact of the bottlenck introduced by the access to a shared state
- normal_form - implementation of the normal form: Pipe(Comp(f1, f2, ...))

#### std_parallel
two implementations with std C++ concurrency mechanisms
- async - use of async calls with no thread pinning
 -thread - use of thread calls with thread pinning (best performances)

#### scripts
three python scripts
- time.py - takes as input an implementation and its argument and measures the execution time varying the number of workers.
        e.g python ./time.py ../ff_parallel/normal_form/solve_tsp 10000 1000 20 256 1234 executes the algorithm varying nw from 1 to 256
        the resulting list is saved in a pickle file
- best_count.py - executes the inputted implementation 100 times and counts how many time the optimal solution was found
        the syntax is the same one of time.py
- make_graph.py - reads the pickle file generated from time.py and produced a png graph.
        the scritp can be modified to plot speedup, scalability or efficiency.

