#include <iostream>
#include <functional>
#include <vector>
#include <sstream>
#include <ff/ff.hpp>
#include <ff/pipeline.hpp>
#include <ff/farm.hpp>

#include "tsp.cpp"
#include "utils.cpp"
#include "genetic.cpp"

using namespace std;
using namespace ff;

struct Genetic: ff_node_t<std::vector<int>> {
    Genetic(int k, int gen, int p, function<bool(vector<int>, vector<int>)> cmp, unsigned s): k(k), gen(gen), p(p), cmp(cmp), s(s) {}
    function<bool(vector<int>, vector<int>)> cmp;
    int k, gen, p;
    unsigned s;

    vector<int>* svc(vector<int>*) {
        int acc1 = 0;
        int acc2 = 0;

        // rand_x r;
        // for(int i = 0; i < get_my_id() * 10; i++){
        //     auto x = intRand(0, p);
        //     auto y = r(p);
        // }
        
        auto chsomes = get_first_generation(k, p - 1);

        // cout << "worker " << get_my_id() << ": ";
        // print_vector(chsomes[0]);

        vector<vector<int>> new_chsomes(k);
        make_heap(chsomes.begin(), chsomes.end(), cmp);

        auto t0 = chrono::system_clock::now();
        for(int i = 0; i < gen; i++){  
            auto t1 = chrono::system_clock::now();
            evolve(chsomes, new_chsomes);
            acc1 += chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now() - t1).count();
            auto t2 = chrono::system_clock::now();
            natural_selection(chsomes, new_chsomes, cmp);
            acc2 += chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now() - t2).count();
        }
        auto elapsed = chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now() - t0).count();
        std::ostringstream st;
        st << "Worker " << get_my_id() << " Time: " << elapsed << " us"; 
        st << " Evolve: " << acc1  << " us";
        st << " Select: " << acc2  << " us" << endl;
        cout << st.str();
        sort_heap(chsomes.begin(), chsomes.end(), cmp);
        vector<int>* res = new vector<int>(chsomes.front());
        return res;
    }
};

struct CollectSelect: ff_node_t<std::vector<int>> {
    CollectSelect(function<bool(vector<int>, vector<int>)> cmp): cmp(cmp) {}
    function<bool(vector<int>, vector<int>)> cmp;
    vector<int>* best = NULL;

    vector<int>* svc(vector<int>* chsome) {
        if(best == NULL)
            best = chsome;
        else
            best = cmp(*chsome, *best) ? chsome : best;
        return GO_ON;
    }
};


int main(int argc, char** argv) {
        //Nota: il nodo di partenza e di arrivo è sempre 0
    if(argc != 6){
        cout << "Wrong number of parameters" << endl;
        exit(1);
    }
    int k = atoi(argv[1]); // population size
    int g = atoi(argv[2]); // generations
    int p  = atoi(argv[3]); // problem dimension
    int nw = atoi(argv[4]); //number of workers
    int s = atoi(argv[5]); // seed

    srand(s);
    // Problem Setup
    TspGraph graph(p);
    // Set a best path so we can be sure the algorithm finds it
    vector<int> best(p - 1);
    iota(best.begin(), best.end(), 1);
    random_shuffle (best.begin(), best.end() );
    graph.set_best_path(best);
    
    // Compare function
    auto cmp = [&](vector<int> x, vector<int> y) { return (graph.path_length(x)) < (graph.path_length(y)); };

    ff_Farm<float> farm( [&]() {
        std::vector<std::unique_ptr<ff_node> > W;
        for(size_t i=0;i<nw;++i) W.push_back(make_unique<Genetic>(k / nw, g, p, cmp, s));
        return W;
    } () );

    CollectSelect cs(cmp);
    farm.add_collector(cs);

    long time;

    {
        utimer ts{"Total time", &time};
        farm.run_and_wait_end();
    }
 
    cout << "Best path found: " << graph.path_length(*cs.best) << endl;
}
