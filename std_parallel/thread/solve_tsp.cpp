#include <iostream>
#include <numeric>
#include <vector>
#include <thread>
#include <future>

#include "tsp.cpp"
#include "utils.cpp"
#include "genetic.cpp"

using namespace std;

// #define DEBUG

struct Worker{

    vector<unsigned char>* res = NULL;
    Worker(){}

    void genetic_loop(int k, int g, int p, int s, function<bool(vector<unsigned char>, vector<unsigned char>)> cmp, int id){

        int acc1 = 0;
        int acc2 = 0;
        Rand r(s + id);
            
        auto chsomes = get_first_generation(k, p - 1, r);
        vector<vector<unsigned char>> new_chsomes(k);
        make_heap(chsomes.begin(), chsomes.end(), cmp);
        #ifdef DEBUG
        auto t0 = chrono::system_clock::now();
        #endif
        for(int i = 0; i < g; i++){  
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
        st << "Worker " << id << " Time: " << elapsed << " us"; 
        st << " Evolve: " << acc1  << " us";
        st << " Select: " << acc2  << " us" << endl;
        cout << st.str();
        #endif
        sort_heap(chsomes.begin(), chsomes.end(), cmp);
        res = new vector<unsigned char>(chsomes.front());
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

    k = k / nw;

    srand(s);
    // Problem Setup
    TspGraph graph(p);
    // Set a best path so we can be sure the algorithm finds it
    vector<unsigned char> best_path(p - 1);
    iota(best_path.begin(), best_path.end(), 1);
    random_shuffle (best_path.begin(), best_path.end() );
    graph.set_best_path(best_path);
    
    // Compare function
    auto cmp = [&](vector<unsigned char> x, vector<unsigned char> y) { return (graph.path_length(x)) < (graph.path_length(y)); };

    long time;
    {
        utimer ts{"Total time", &time};

        vector<thread> threads(nw);
        std::vector<std::unique_ptr<Worker>> W(nw);

        for(int i = 0; i < nw; i++){
            W[i] = make_unique<Worker>();
            threads[i] = std::thread([&, i]() { W[i]->genetic_loop(k, g, p, s, cmp, i); } );

            cpu_set_t cpuset;
            CPU_ZERO(&cpuset);
            CPU_SET(i, &cpuset);
            pthread_setaffinity_np(threads[i].native_handle(), sizeof(cpu_set_t), &cpuset);
        }

        vector<unsigned char>* best = NULL; 

        for (int i = 0; i < nw; i++) {
            threads[i].join();
            auto chsome = W[i]->res;

            if(best == NULL)
                best = chsome;
            else
                best = cmp(*chsome, *best) ? chsome : best;
        }
        cout << "Best path found: " << graph.path_length(*best) << endl;
    }
}