#ifndef LIEF_H
#define LIEF_H
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <random>
#include <sstream>
#include <system_error>
#include <vector>

class life {
private:
  int H, W;
  long long size;
  std::vector<int> *currentLife = new std::vector<int>;
  std::vector<int> *nextLife = new std::vector<int>;
  std::string rule;

public:
  // initialise
  life(){};
  life(const int &height, const int &width) {
    H = height;
    W = width;
    size = height * width;
    for (int i = 0; i < size; ++i) {
      currentLife->push_back(0);
      nextLife->push_back(0);
    }
  }

  life(const int &height, const int &width,
       const int &N, const long &seed) :
      life(height, width) {
    auto number = (N < size) ? N : size << 1;
    for (int i = 0; i < number;
         (*currentLife)[i++] = 1) {
    }
    std::shuffle(currentLife->begin(), currentLife->end(),
                 std::default_random_engine(seed));
  }
  //
  life(const int &height, const int &width, const std::vector<int> &conf) {
    H = height;
    W = width;
    size = height * width;
    for (auto &e : conf) {
      currentLife->push_back(e);
      nextLife->push_back(e);
    }
  }
  life(const life &other) {
    this->H = (other.H);
    this->W = (other.W);
    this->size = (other.size);
    this->currentLife = (other.currentLife);
    this->nextLife = (other.nextLife);
  }

  life(const std::string &file, int edge = 10) {
    this->read(file, edge);
  }

  life &operator=(const life &other) {
    this->H = (other.H);
    this->W = (other.W);
    this->size = (other.size);
    this->currentLife = (other.currentLife);
    this->nextLife = (other.nextLife);
    return *this;
  }

  ~life() {
    delete currentLife;
    delete nextLife;
  }
  // life(const int i, const int j, const int &Setup)

  //methods
  int getNeighbor(const int &index, bool bound);
  void update(int n, bool bound);
  void check();
  void print();
  int *getData() {
    return (*currentLife).data();
  }

  int getH() {
    return this->H;
  }

  int getW() {
    return this->W;
  }
  long long _size() {
    return static_cast<long long>(size);
  }
  // read rle file
  void read(const std::string &, int);
};

#endif