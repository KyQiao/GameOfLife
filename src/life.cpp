#include "life.h"
#include <iostream>
#include <vector>
#include <random>
#include <iterator>
#include <algorithm>
#include <omp.h>


life::life(const int i, const int j) {
    init(i, j);
}

life::life(const int i, const int j, int N, const long seed) {
    init(i, j);

    if (N > H * W) {
        std::cout << "input too large" << std::endl;
        N = 32;
    }

    for (int i = 0; i < N; ++i) {
        currectLife[i] = 1;
    }

    std::shuffle (currectLife.begin (), currectLife.end (), std::default_random_engine (seed));

}

void life::init(const int i, const int j) {
    H = i;
    W = j;
    size = H * W;
    for (int i = 0; i < H * W; ++i) {
        currectLife.push_back(0);
        nextLife.push_back(0);
    }
}

void life::check() {
    int sum = 0;
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            std::cout << currectLife[H * i + j] << " ";
            sum += currectLife[H * i + j];
        };
        std::cout << "\n";
    }
    std::cout << "\n";
    std::cout << "Number of live: " << sum << std::endl;
}

void life::print() {
    int sum = 0;
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            std::cout << currectLife[H * i + j] << " ";
            sum += currectLife[H * i + j];
        };
        std::cout << "\n";
    }
    std::cout << "\n";
}

int life::getNeighbor(const int index) {
    int sum = 0;
    std::vector<int> v = {index - W - 1, \
                          index - W, \
                          index - W + 1, \
                          index - 1, \
                          index + 1, \
                          index + W - 1, \
                          index + W, \
                          index + W + 1
                         };
    for (int &i : v) {
        i += size;
        i = i % size;
        sum += currectLife[i];
        // std::cout << i << std::endl;
    };
    return sum;
}

void life::update() {
    int neighor;
    #pragma omp parallel for
    for (int i = 0; i < size; ++i) {
        neighor = getNeighbor(i);
        if (neighor < 2) nextLife[i] = 0;
        else if (neighor > 3) nextLife[i] = 0;
        else if (currectLife[i] == 0 && neighor == 3) nextLife[i] = 1;
    }
    currectLife = nextLife;
}

void life::update(const int n) {
    int neighor;
    omp_set_num_threads(n);
    #pragma omp parallel for
    for (int i = 0; i < size; ++i) {
        neighor = getNeighbor(i);
        if (neighor < 2) nextLife[i] = 0;
        else if (neighor > 3) nextLife[i] = 0;
        else if (currectLife[i] == 0 && neighor == 3) nextLife[i] = 1;
    }
    currectLife = nextLife;
}