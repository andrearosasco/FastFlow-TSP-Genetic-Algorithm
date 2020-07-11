#include <vector>
#include <iostream>
#include <numeric>
#include <cstdlib>

class TspGraph {
    private:
        int** graph;
        int n;
    public:
    TspGraph(int n): n(n) {
        graph = new int*[n];
        for (int i = 0; i < n; i++)
            graph[i] = new int[n];

        for(int i = 0; i < n; i++){
            graph[i][i] = 0;
            for(int j = i + 1; j < n; j++){
                int w = (rand() % 99) + 2;
                graph[i][j] = w;
                graph[j][i] = w;
            }
        }
    }

    int path_length(std::vector<int> path){
        int length = 0;

        length += graph[0][path.front()];
        for(int i = 0; i < path.size() - 1; i++){
            int to = path[i];
            int from = path[i + 1];
            int l = graph[to][from];
            length += l;
        }
        length += graph[path.back()][0];
        return length;
    }
};

int main(){
    int n = 10;

    TspGraph graph(n);

    std::vector<int> path(n);
    std::iota(path.begin(), path.end(), 0);

    length = graph.path_length(path)
    std::cout << length << std::endl;
    return 0;
}