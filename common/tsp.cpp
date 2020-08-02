#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

class TspGraph {
    private:
        unsigned char** graph;
        int n;
    public:
    TspGraph(int n): n(n) {
        graph = new unsigned char*[n];
        for (int i = 0; i < n; i++)
            graph[i] = new unsigned char[n];

        for(int i = 0; i < n; i++){
            graph[i][i] = 0;
            for(int j = i + 1; j < n; j++){
                int w = (rand() % 99) + 2;
                graph[i][j] = w;
                graph[j][i] = w;
            }
        }
    }

    void set_best_path(vector<unsigned char> path) {
        graph[0][path.front()] = 1;
        graph[path.front()][0] = 1;

        for(int i = 0; i < path.size() - 1; i++){
            graph[path[i]][path[i+1]] = 1;
            graph[path[i+1]][path[i]] = 1;
        }
        graph[path.back()][0] = 1;
        graph[0][path.back()] = 1;
    }

    int path_length(vector<unsigned char> path){
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

    void print(){
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                cout << setw(4) << graph[i][j] << " ";
            }
            cout << endl;
        }
    }
};