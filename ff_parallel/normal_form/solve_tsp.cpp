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

// #define DEBUG

struct Genetic: ff_node_t<std::vector<unsigned char>> {
    Genetic(int k, int gen, int p, function<bool(vector<unsigned char>, vector<unsigned char>)> cmp, unsigned s): k(k), gen(gen), p(p), cmp(cmp), s(s) {}
    function<bool(vector<unsigned char>, vector<unsigned char>)> cmp;
    int k, gen, p;
    unsigned s;

    vector<unsigned char>* svc(vector<unsigned char>*) {
        int acc1 = 0;
        int acc2 = 0;
        
        Rand r(s + get_my_id());
        auto chsomes = get_first_generation(k, p - 1, r);

        vector<vector<unsigned char>> new_chsomes(k);
        make_heap(chsomes.begin(), chsomes.end(), cmp);

        #ifdef DEBUG
            auto t0 = chrono::system_clock::now();
        #endif
        for(int i = 0; i < gen; i++){  
            #ifdef DEBUG
                auto t1 = chrono::system_clock::now();
            #endif
            evolve(chsomes, new_chsomes, r);
            #ifdef DEBUG
                acc1 += chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now() - t1).count();
                auto t2 = chrono::system_clock::now();
            #endif
            natural_selection(chsomes, new_chsomes, cmp);
            #ifdef DEBUG
                acc2 += chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now() - t2).count();
            #endif
        }
        #ifdef DEBUG
            auto elapsed = chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now() - t0).count();
            std::ostringstream st;
            st << "Worker " << get_my_id() << " Time: " << elapsed << " us"; 
            st << " Evolve: " << acc1  << " us";
            st << " Select: " << acc2  << " us" << endl;
            cout << st.str();
        #endif
        sort_heap(chsomes.begin(), chsomes.end(), cmp);
        vector<unsigned char>* res = new vector<unsigned char>(chsomes.front());
        return res;
    }
};

struct CollectSelect: ff_node_t<std::vector<unsigned char>> {
    CollectSelect(function<bool(vector<unsigned char>, vector<unsigned char>)> cmp): cmp(cmp) {}
    function<bool(vector<unsigned char>, vector<unsigned char>)> cmp;
    vector<unsigned char>* best = NULL;

    vector<unsigned char>* svc(vector<unsigned char>* chsome) {
        if(best == NULL)
            best = chsome;
        else
            best = cmp(*chsome, *best) ? chsome : best;
        return GO_ON;
    }
};


int main(int argc, char** argv) {

    if(argc != 6){
        cout << "Wrong number of parameters" << endl;
        exit(1);
    }
    int k = atoi(argv[1]); // population size
    int g = atoi(argv[2]); // generations
    int p  = atoi(argv[3]); // problem dimension
    int nw = atoi(argv[4]); //number of workers
    int s = atoi(argv[5]); // seed

    k = k / nw; //uncomment to measure parallel performance 

    srand(s);
    // Problem Setup
    TspGraph graph(p);
    // Set a best path so we can be sure the algorithm finds it
    vector<unsigned char> best(p - 1);
    iota(best.begin(), best.end(), 1);
    random_shuffle (best.begin(), best.end() );
    graph.set_best_path(best);
    
    // Compare function
    auto cmp = [&](vector<unsigned char> x, vector<unsigned char> y) { return (graph.path_length(x)) < (graph.path_length(y)); };

    ff_Farm<float> farm( [&]() {
        std::vector<std::unique_ptr<ff_node> > W;
        for(size_t i=0;i<nw;++i) W.push_back(make_unique<Genetic>(k, g, p, cmp, s));
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
