#include <iostream>
#include <vector>

#include "utils.cpp"
#include "tsp.cpp"
#include "genetic.cpp"

using namespace std;

// #define DEBUG

int main(int argc, char * argv[]) {
    //Nota: il nodo di partenza e di arrivo è sempre 0
    if(argc != 5){
        cout << "Wrong number of parameters" << endl;
        exit(1);
    }
    int k = atoi(argv[1]); // population size
    int g = atoi(argv[2]); // generations
    int p  = atoi(argv[3]); // problem dimension
    int s = atoi(argv[4]); // seed for random weight

    srand(s);
    Rand r(s);
    // Problem Setup
    TspGraph graph(p);
    // Set a best path so we can be sure the algorithm finds it
    vector<unsigned char> best(p - 1);
    iota(best.begin(), best.end(), 1);
    random_shuffle (best.begin(), best.end() );
    graph.set_best_path(best);
    // Evolution Loop
    auto cmp = [&](vector<unsigned char> x, vector<unsigned char> y) { return (graph.path_length(x)) < (graph.path_length(y)); };

    auto chsomes = get_first_generation(k, p - 1, r);
    make_heap(chsomes.begin(), chsomes.end(), cmp);
    vector<vector<unsigned char>> new_chsomes(k);
    int i;
    long exect;
    #ifdef DEBUG
    long acc = 0;
    long acc2 = 0;
    #endif
    auto t0 = chrono::system_clock::now();
    for(i = 0; i < g; i++){   
        #ifdef DEBUG   
        auto t1 = chrono::system_clock::now();
        #endif
        evolve(chsomes, new_chsomes, r);
        #ifdef DEBUG
        acc += chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now() - t1).count();
        auto t2 = chrono::system_clock::now();
        #endif
        natural_selection(chsomes, new_chsomes, cmp);
        #ifdef DEBUG
        acc2 += chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now() - t2).count();
        #endif
    }

    auto elapsed = chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now() - t0).count();
    cout << "Time: " << elapsed << endl;
    #ifdef DEBUG
    cout << "Evolve: " << acc << endl;
    cout << "Selection: " << acc2 << endl;
    #endif

    cout << endl << "Number of possible hamiltonian cycles: " << factorial(p - 1) / 2 << endl;

    cout << endl << "Tested solutions: " << k * i << endl;
    cout << "Iterations: " << i << endl;
    cout << "Population: " << k << endl;

    cout << endl << "Best path: " << p << endl;
    
    
    sort_heap(chsomes.begin(), chsomes.end(), cmp);
    cout << "Best path found: " << graph.path_length(chsomes.front()) << endl;

}
