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

//possibilmente da vettorializzare
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


vector<vector<int>> evolve(const vector<vector<int>>& chsomes) {
    vector<vector<int>> res(chsomes);

    int po_crossover = 70; // percentuale di cromosomi per crossover (gli altri vengono mutati)
    int no_crossover = round(res.size() * po_crossover / 200.) * 2; //arrotonda al numero pari più vicino

    random_shuffle (res.begin(), res.end(), rand_x());

    for(int j = 0; j < res.size(); j++) {
        if(j < no_crossover){
            auto aux = crossover(res[j], res[j+1]); j++;
            res[j] = *aux[0];
            res[j+1] = *aux[1];
        } else {
            auto aux = mutation(res[j]);
            res[j] = *aux;
        }
    }
    return res;
}

void natural_selection(vector<vector<int>>& g1, const vector<vector<int>>& g2, function<bool(vector<int>, vector<int>)> cmp) {
    for(int i = 0; i < g2.size(); i++){
        if( cmp(g2[i], g1.front()) ){
            pop_heap(g1.begin(), g1.end(), cmp);
            g1.pop_back();
            g1.push_back(g2[i]);
            push_heap(g1.begin(), g1.end(), cmp);
        }
    }
}
