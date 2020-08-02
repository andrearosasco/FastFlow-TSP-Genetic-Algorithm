#include <vector>
#include <iostream>
#include <chrono>
#include <functional>
#include <mutex>
#include <shared_mutex>
#include <random>
#include <atomic>

using namespace std;

void print_vector(std::vector<unsigned char> v){
    for(int i = 0; i < v.size(); i++){
        std::cout << +v[i] << " ";
    }
    std::cout << std::endl;
}

bool has_duplicates(std::vector<unsigned char> v){
    sort(v.begin(), v.end());
    bool wasUnique = std::adjacent_find(v.begin(), v.end()) != v.end();
    return wasUnique;
}

double factorial(int n) {
    double res = 1;
    for (int i = 0; i < n; i++){
        res *= (i + 1);
    }
    return res;
}


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

struct Rand{
  unsigned seed;
  Rand(unsigned seed): seed(seed){}

  int intRand(const int & min, const int & max) {
          static thread_local std::mt19937 generator(seed);
          std::uniform_int_distribution<int> distribution(min,max);
          return distribution(generator);
  }

  int operator()(int limit) {
        static thread_local std::mt19937 generator(seed);
        std::uniform_int_distribution<int> distribution(0,limit-1);
        return distribution(generator);
    }
};
