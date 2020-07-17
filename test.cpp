#include "utils.cpp"

int main(){
    Rand r(time(NULL));
    cout << r(10) << endl;

    return 0;
}