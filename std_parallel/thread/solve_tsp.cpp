#include <iostream>
#include <numeric>
#include <vector>
#include <thread>
#include <future>

#include "tsp.cpp"
#include "utils.cpp"
#include "genetic.cpp"

using namespace std;

void genetic_loop(int k, int g, int p, function<bool(vector<int>, vector<int>)> cmp, int id, vector<int>* res){

    int acc1 = 0;
    int acc2 = 0;
        
    auto chsomes = get_first_generation(k, p - 1);
    vector<vector<int>> new_chsomes(k);
    make_heap(chsomes.begin(), chsomes.end(), cmp);

    auto t0 = chrono::system_clock::now();
    for(int i = 0; i < g; i++){  
        auto t1 = chrono::system_clock::now();
        evolve(chsomes, new_chsomes);
        acc1 += chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now() - t1).count();
        auto t2 = chrono::system_clock::now();
        natural_selection(chsomes, new_chsomes, cmp);
        acc2 += chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now() - t2).count();
    }
    auto elapsed = chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now() - t0).count();
    std::ostringstream st;
    st << "Worker " << id << " Time: " << elapsed << " us"; 
    st << " Evolve: " << acc1  << " us";
    st << " Select: " << acc2  << " us" << endl;
    cout << st.str();
    sort_heap(chsomes.begin(), chsomes.end(), cmp);
    res = new vector<int>(chsomes.front());
    print_vector(*res);
}

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
    vector<int> best_path(p - 1);
    iota(best_path.begin(), best_path.end(), 1);
    random_shuffle (best_path.begin(), best_path.end() );
    graph.set_best_path(best_path);
    
    // Compare function
    auto cmp = [&](vector<int> x, vector<int> y) { return (graph.path_length(x)) < (graph.path_length(y)); };

    vector<int> best;
    long time;
    {
        utimer ts{"Total time", &time};

        vector<thread> threads(nw);
        vector<vector<int>*> res(nw);
        for(int i = 0; i < nw; i++){
            vector<int>* r;
            res[i] = r;
            threads[i] = std::thread([&]() { genetic_loop(k / nw, g, p, cmp, i, r); } );

            cpu_set_t cpuset;
            CPU_ZERO(&cpuset);
            CPU_SET(i, &cpuset);
            pthread_setaffinity_np(threads[i].native_handle(), sizeof(cpu_set_t), &cpuset);
        }

        for (int i = 0; i < nw; i++) {
            // cout << "Joining " << i << endl;
            threads[i].join();
            // cout << "Joined " << i << endl;

            // auto chsome = *(res[i]);
            // print_vector(chsome);
            // if(best.size() == 0)
            //     best = chsome;
            // else
            //     best = cmp(chsome, best) ? chsome : best;
        }
    }
    // cout << "Best path found: " << graph.path_length(best) << endl;
}