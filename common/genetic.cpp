#include <vector>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <queue>
#include <math.h>

using namespace std;

vector<vector<unsigned char>> get_first_generation(int population_size, int gene_size, Rand r){
    vector<vector<unsigned char>> res(population_size);
    for(int i = 0; i < population_size; i++) {
        vector<unsigned char> gene(gene_size);
        iota(gene.begin(), gene.end(), 1);
        random_shuffle ( gene.begin(), gene.end(), r);
        res[i] = gene;
    }
    return res;
}


vector<vector<unsigned char>> crossover(const vector<unsigned char>& g1, const vector<unsigned char>& g2, Rand r){

    vector<unsigned char> ng1 = g1;
    vector<unsigned char> ng2 = g2;

    int p = r.intRand(0, g1.size() - 1);
    int t = r.intRand(p, g1.size() - 1);

    int p1 = p;
    int p2 = p;

    for(int i = 0; i < g1.size() && (p1 <= t || p2 <= t) ; i++){

        // find cerca tra p e t e restituisce t + 1 se non trova l'elemento
        if(find(g1.begin() + p, g1.begin() + (t + 1), g2[i]) != g1.begin() + (t + 1)){
            ng1[p1++] = g2[i];
        }
        if(find(g2.begin() + p, g2.begin() + (t + 1), g1[i]) != g2.begin() + (t + 1)){
            ng2[p2++] = g1[i];
        }
    }

    vector<vector<unsigned char>> res(2);
    res[0] = ng1;
    res[1] = ng2;
    return res;
}

vector<unsigned char> mutation(const vector<unsigned char>& g, Rand r) {
    vector<unsigned char> res = g;

    auto l = 1;

    for(int i = 0; i < l; i++){
        int p = r.intRand(0, g.size() - 1);
        int t = r.intRand(0, g.size() - 1);

        auto aux = res[p];
        res[p] = res[t];
        res[t] = aux;
    }
    return res;
}


void evolve(const vector<vector<unsigned char>>& chsomes, vector<vector<unsigned char>>& res, Rand r) {

    for(int i = 0; i < chsomes.size(); i++){
        res[i] = chsomes[i];
    }

    int po_crossover = 70; // percentuale di cromosomi per crossover (gli altri vengono mutati)
    int no_crossover = round(res.size() * po_crossover / 200.) * 2; //arrotonda al numero pari più vicino

    random_shuffle (res.begin(), res.end(), r);

    for(int j = 0; j < res.size(); j++) {
        if(j < no_crossover){
            auto aux = crossover(res[j], res[j+1], r); j++;
            res[j] = aux[0];
            res[j+1] = aux[1];
        } else {
            auto aux = mutation(res[j], r);
            res[j] = aux;
        }
    }
}

void natural_selection(vector<vector<unsigned char>>& g1, const vector<vector<unsigned char>>& g2, function<bool(vector<unsigned char>, vector<unsigned char>)> cmp) {
    for(int i = 0; i < g2.size(); i++){
        if( cmp(g2[i], g1.front()) ){
            pop_heap(g1.begin(), g1.end(), cmp);
            g1.pop_back();
            g1.push_back(g2[i]);
            push_heap(g1.begin(), g1.end(), cmp);
        }
    }
}
