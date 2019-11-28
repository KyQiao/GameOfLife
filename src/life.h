#include <iostream>
#include <vector>
#include <iterator>

class life {
  public:
    int H, W;
    int size;
    std::vector<bool> currectLife;
    std::vector<bool> nextLife;

    // initialise
    life(const int i, const int j);
    life(const int i, const int j, int N, const long seed);
    // life(const int i, const int j, const int &Setup)

    //methods
    int getNeighbor(const int index);

    void update();
    void update(const int n);

    void init(const int i, const int j);

    void check();
    void print();
};


