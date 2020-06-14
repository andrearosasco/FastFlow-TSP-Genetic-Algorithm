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
        random_shuffle ( gene->begin(), gene->end() );
        res[i] = *gene;
    }
    return res;
}

//possibilmente da vettorializzare
vector<vector<int>*> crossover(const vector<int>& g1, const vector<int>& g2){

    vector<int> *ng1 = new vector<int>(g1);
    vector<int> *ng2 = new vector<int>(g2);

    int p = rand() % g1.size();
    int t = p + (rand() % (g1.size() - p));

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
        int p = rand() % g.size();
        int t = rand() % g.size();

        auto aux = (*res)[p];
        (*res)[p] = (*res)[t];
        (*res)[t] = aux;
    }
    return res;
}


vector<vector<int>> evolve(vector<vector<int>> genes) {
    vector<vector<int>> res;
    int po_crossover = 92; // percentuale di cromosomi per crossover (gli altri vengono mutati)
    int no_crossover = round(genes.size() * po_crossover / 200.) * 2; //arrotonda al numero pari più vicino


    int s = 0;
    bool ready = false;
    vector<int> g1;
    for(int i = 0; i < genes.size(); i++) {
        float p = (rand() % 100) / 100.;
        if(p < (no_crossover - s) / (float)(genes.size() - i)){
            if(!ready) // eseguiamo il crossover solo quando abbaimo una coppia di geni
                g1 = genes[i];
            else {
                auto co = crossover(g1, genes[i]);
                res.push_back(*co[0]);
                res.push_back(*co[1]);
            }
            ready = !ready;
            s++;
        }
        else {
            res.push_back(*mutation(genes[i]));
        }
    }
    return res;
}

vector<vector<int>> natural_selection(vector<vector<int>> g1, vector<vector<int>> g2, TspGraph graph) {
    auto cmp = [&](vector<int> x, vector<int> y) { return (graph.path_length(x)) < (graph.path_length(y)); };
    float w = 0;
    for(int i = 0; i < g2.size(); i++){
        if( graph.path_length(g2[i]) < graph.path_length(g1.front()) ){
            w++;
            pop_heap(g1.begin(), g1.end(), cmp);
            g1.pop_back();
            g1.push_back(g2[i]);
            push_heap(g1.begin(), g1.end(), cmp);
        }
    }

    return g1;
}
