#include "life.h"
#include <iostream>
#include <omp.h>



void life::check() {
    int sum = 0;
	for (int i = 0; i < size; sum += (*currentLife)[i++]) {}
    std::cout << "\n";
    std::cout << "Number of live: " << sum << std::endl;
}

void life::print() {
    int sum = 0;
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            std::cout << (*currentLife)[H * i + j] << " ";
            sum += (*currentLife)[H * i + j];
        };
        std::cout << "\n";
    }
    std::cout << "\n";
}

int life::getNeighbor(const int& index) {
    int sum = 0;
    //make them inorder to use cache
    //plus size to deal boundray condition
    sum += (*currentLife)[(index - W - 1 +size)%size];
    sum += (*currentLife)[(index - W + size) % size];
    sum += (*currentLife)[(index - W + 1 + size) % size];
    sum += (*currentLife)[(index - 1 + size) % size];
    sum += (*currentLife)[(index + 1 + size) % size];
    sum += (*currentLife)[(index + W - 1) % size];
    sum += (*currentLife)[(index + W + size) % size];
    sum += (*currentLife)[(index + W + 1 + size) % size];
    return sum;
}


void life::update(int n) {
    int neighor;
    omp_set_num_threads(n);
    #pragma omp parallel for
    for (int i = 0; i < size; ++i) {
        neighor = getNeighbor(i);
        if (neighor < 2) (*nextLife)[i] = 0;
        else if (neighor > 3) (*nextLife)[i] = 0;
        else if ((*currentLife)[i] == 0 && neighor == 3) (*nextLife)[i] = 1;
    }
    //std::vector<int>* tmp= currentLife;
    //currentLife = nextLife;
	//nextLife = tmp;
    *currentLife = *nextLife;
}