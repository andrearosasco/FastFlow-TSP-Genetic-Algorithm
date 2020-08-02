#include <vector>
#include <iostream>
#include <chrono>
#include <functional>
#include <mutex>
#include <shared_mutex>
#include <random>
#include <atomic>

using namespace std;


#define START(timename) auto timename = std::chrono::system_clock::now();
#define STOP(timename,elapsed)  auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - timename).count();

class utimer {
  std::chrono::system_clock::time_point start;
  std::chrono::system_clock::time_point stop;
  std::string message; 
  using usecs = std::chrono::microseconds;
  using msecs = std::chrono::milliseconds;

private:
  long * us_elapsed;
  
public:

  utimer(const std::string m) : message(m),us_elapsed((long *)NULL) {
    start = std::chrono::system_clock::now();
  }
    
  utimer(const std::string m, long * us) : message(m),us_elapsed(us) {
    start = std::chrono::system_clock::now();
  }

  ~utimer() {
    stop =
      std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed =
      stop - start;
    auto musec =
      std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    
    std::cout << message << " computed in " << musec << " usec " 
	      << std::endl;
    if(us_elapsed != NULL)
      (*us_elapsed) = musec;
  }
};

std::atomic<int> acc_insert = 0;
class SynchHeap {
  private:
    function<bool(vector<int>, vector<int>)> cmp;
    vector<vector<int>> heap;
    mutable shared_mutex mutex;
  public:
    SynchHeap(vector<vector<int>> heap, function<bool(vector<int>, vector<int>)> cmp):heap(heap), cmp(cmp){
      make_heap(this->heap.begin(), this->heap.end(), cmp);
    }

    bool check(vector<int> e){
      mutex.lock_shared();
      auto f = heap.front();
      mutex.unlock_shared();
      return cmp(e, f);;
    }

    void insert(vector<int> e){
      auto t0 = chrono::system_clock::now();
      mutex.lock();
      acc_insert += chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now() - t0).count();
      if(cmp(e, heap.front())){
        pop_heap(heap.begin(), heap.end(), cmp); heap.pop_back();
        heap.push_back(e); push_heap(heap.begin(), heap.end(), cmp);
      }
      mutex.unlock();
    }

    vector<int> operator [](int i) const    {return heap[i];}
    vector<int> & operator [](int i) {return heap[i];}

    int size() {
      return heap.size();}

    vector<vector<int>> get_v() {
      return heap;
    }

    void sort(){
      sort_heap(heap.begin(), heap.end(), cmp);
    }
};


class rand_x { 

public:
    rand_x() {}

    int operator()(int limit) {
        static thread_local std::mt19937 generator;
        std::uniform_int_distribution<int> distribution(0,limit-1);
        return distribution(generator);
    }
};


int intRand(const int & min, const int & max) {
        static thread_local std::mt19937 generator;
        std::uniform_int_distribution<int> distribution(min,max);
        return distribution(generator);
}
