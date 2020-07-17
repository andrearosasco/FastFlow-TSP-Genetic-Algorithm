#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

#include "tsp.cpp"

using namespace std;

int main(int argc, char** argv) {
        //Nota: il nodo di partenza e di arrivo è sempre 0
    if(argc != 2){
        cout << "Wrong number of parameters" << endl;
        exit(1);
    }

    int p  = atoi(argv[1]); // problem dimension

    srand(time(NULL));
    // Problem Setup
    TspGraph graph(p);
    // Set a best path so we can be sure the algorithm finds it
    vector<int> best_path(p - 1);
    iota(best_path.begin(), best_path.end(), 1);
    random_shuffle (best_path.begin(), best_path.end());
    graph.set_best_path(best_path);


    vector<int> path(p - 1);
    iota(path.begin(), path.end(), 1);
    random_shuffle (path.begin(), path.end());
    cout << graph.path_length(path) << endl;
}