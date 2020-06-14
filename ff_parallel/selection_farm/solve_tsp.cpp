#include <iostream>
#include <functional>
#include <vector>
#include <ff/ff.hpp>
#include <ff/pipeline.hpp>
#include <ff/farm.hpp>

#include "tsp.cpp"
#include "utils.cpp"
#include "genetic.cpp"

using namespace std;
using namespace ff;

class chsomesDispatch: public ff_node_t<std::vector<int>> {
    private:
        SynchHeap& heap;
        int g, k;
        int c = 0, i = 0;
        int no_crossover;
    public:
        chsomesDispatch(SynchHeap& heap, int g):heap(heap), g(g), k(heap.size()), no_crossover(round(k * 94 / 200.) * 2) {}
        std::vector<int>* svc(std::vector<int> * task) {
            if (i < g){
                if( task == nullptr || (task != nullptr && ++c == k)){
                    delete task;
                    c = 0;
                    auto chsomes = heap.get_v();
                    random_shuffle (chsomes.begin(), chsomes.end());

                    for(int j = 0; j < k; j++) {
                        if(j < no_crossover){
                            auto res = crossover(chsomes[j], chsomes[j+1]); j++;
                            ff_send_out(res[0]);
                            ff_send_out(res[1]);
                        } else {
                            auto res = mutation(chsomes[j]);
                            ff_send_out(res);
                        }
                    }
                    i++;
                }
                return GO_ON;
            }
            return EOS;
        }
};

struct chsomesSelect: ff_node_t<std::vector<int>> {
    chsomesSelect(SynchHeap& heap): heap(heap) {}

    std::vector<int>* svc(std::vector<int> * new_chsome) {

        if( heap.check(*new_chsome) ){
            heap.insert(*new_chsome);
        }
        return new_chsome;
    }

    SynchHeap& heap;
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
    
    // Initialize heap for selection
    auto genes = get_first_generation(k, p - 1);
    SynchHeap heap(genes, [&](const vector<int>& x, const vector<int>& y) { return (graph.path_length(x) < graph.path_length(y)); });

    ff_Farm<float> farm( [&]() {
        std::vector<std::unique_ptr<ff_node> > W;
        for(size_t i=0;i<nw;++i) W.push_back(make_unique<chsomesSelect>(heap));
        return W;
    } () );

    chsomesDispatch fs(heap, g);
    farm.add_emitter(fs);
    farm.remove_collector();
    farm.wrap_around();

    long time;

    {
        utimer ts{"Farm selection", &time};
        farm.run_and_wait_end();
    }
 
    heap.sort();
    cout << "Best path found: " << graph.path_length(heap.get_v().front()) << endl;
}
