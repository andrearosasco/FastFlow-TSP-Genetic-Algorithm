#include <vector>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <queue>
#include <math.h>

using namespace std;

vector<vector<int>> get_first_generation(int population_size, int gene_size){
    vector<vector<int>> res(population_size);
    for(int i = 0; i < population_size; i++) {
        vector<int> *gene = new vector<int>(gene_size);
        iota(gene->begin(), gene->end(), 1);
        random_shuffle ( gene->begin(), gene->end(), rand_x() );
        res[i] = *gene;
    }
    return res;
}


vector<vector<int>*> crossover(const vector<int>& g1, const vector<int>& g2){

    vector<int> *ng1 = new vector<int>(g1);
    vector<int> *ng2 = new vector<int>(g2);

    int p = intRand(0, g1.size() - 1);
    int t = intRand(p, g1.size() - 1);

    int p1 = p;
    int p2 = p;

    for(int i = 0; i < g1.size() && (p1 <= t || p2 <= t) ; i++){

        // find cerca tra p e t e restituisce t + 1 se non trova l'elemento
        if(find(g1.begin() + p, g1.begin() + (t + 1), g2[i]) != g1.begin() + (t + 1)){
            (*ng1)[p1++] = g2[i];
        }
        if(find(g2.begin() + p, g2.begin() + (t + 1), g1[i]) != g2.begin() + (t + 1)){
            (*ng2)[p2++] = g1[i];
        }
    }

    vector<vector<int>*> res(2);
    res[0] = ng1;
    res[1] = ng2;
    return res;
}

vector<int>* mutation(const vector<int>& g) {
    vector<int> *res = new vector<int>(g);

    auto l = max(1, (int)round(g.size() * 0.2));
    for(int i = 0; i < l; i++){
        int p = intRand(0, g.size() - 1);
        int t = intRand(0, g.size() - 1);

        auto aux = (*res)[p];
        (*res)[p] = (*res)[t];
        (*res)[t] = aux;
    }
    return res;
}