#ifndef LIEF_H
#define LIEF_H
#include <iostream>
#include <vector>
#include <random>
#include <iterator>
#include <algorithm>

class life {
private:
    int H, W;
    size_t size;
    std::vector<int>* currentLife = new std::vector<int>;
    std::vector<int>* nextLife = new std::vector<int>;
public:
	// initialise
    life(const int& i, const int& j) {
        H = i;
        W = j;
        size = i * j;
        for (int i = 0; i < size; ++i) {
            currentLife->push_back(0);
            nextLife->push_back(0);
        }
    }

    life(const int& i, const int& j,
        const int& N, const long& seed)
        :life(i, j) {
        auto number = (N < size) ? N : size << 1;
		for (int i = 0; i < number;
            (*currentLife)[i++] = 1){ }
		std::shuffle(currentLife->begin(), currentLife->end(),
            std::default_random_engine(seed));
    }
    life(const int& i, const int& j, const std::vector<int>& conf) {
        H = i;
        W = j;
        size = i * j;
        for (auto& e : conf) {
            currentLife->push_back(e);
            nextLife->push_back(e);
        }
    }
    life(const life& other){
        *currentLife = *(other.currentLife);
        *nextLife = *(other.nextLife);
    }

    life& operator=(const life& other) {
        *currentLife = *(other.currentLife);
        *nextLife = *(other.nextLife);
        return *this;
    }

    ~life()
    {
        delete currentLife;
        delete nextLife;
    }
    // life(const int i, const int j, const int &Setup)

    //methods
    int getNeighbor(const int& index);
    void update(int n=1);
    void check();
    void print();
    int* getData() {
        return (*currentLife).data();
    }
    size_t _size() {
        return size;
    }
};


#endif
