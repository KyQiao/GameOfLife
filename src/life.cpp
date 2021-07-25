#include "life.h"

#include <omp.h>

#include <queue>
#include <regex>

void life::check() {
  int sum = 0;
  for (int i = 0; i < size; sum += (*currentLife)[i++]) {
  }
  std::cout << "\n";
  std::cout << "Number of live: " << sum << std::endl;
}

void life::print() {
  int sum = 0;
  for (int i = 0; i < H; i++) {
    for (int j = 0; j < W; j++) {
      std::cout << (*currentLife)[W * i + j] << " ";
      sum += (*currentLife)[W * i + j];
    };
    std::cout << "\n";
  }
  std::cout << "\n";
}

int life::getNeighbor(const int& index, bool bound) {
  int sum = 0;

  if (!bound) {
    //make them inorder to use cache
    //plus size to deal boundray condition
    sum += (*currentLife)[(index - W - 1 + size) % size];
    sum += (*currentLife)[(index - W + size) % size];
    sum += (*currentLife)[(index - W + 1 + size) % size];
    sum += (*currentLife)[(index - 1 + size) % size];
    sum += (*currentLife)[(index + 1 + size) % size];
    sum += (*currentLife)[(index + W - 1) % size];
    sum += (*currentLife)[(index + W + size) % size];
    sum += (*currentLife)[(index + W + 1 + size) % size];
  } else {
    int x = index % this->W;
    int y = (index - x) / this->W;
    if (x - 1 >= 0) {
      if (y - 1 >= 0) {
        //   left up
        sum += (*currentLife)[(x - 1) + W * (y - 1)];
      }
      if (y + 1 < H) {
        //   left down
        sum += (*currentLife)[(x - 1) + W * (y + 1)];
      }
      //   left center
      sum += (*currentLife)[(x - 1) + W * (y)];
    }

    if (x + 1 < W) {
      if (y - 1 >= 0) {
        //   right up
        sum += (*currentLife)[(x + 1) + W * (y - 1)];
      }
      if (y + 1 < H) {
        //   right down
        sum += (*currentLife)[(x + 1) + W * (y + 1)];
      }
      //   right center
      sum += (*currentLife)[(x + 1) + W * (y)];
    }

    if (y + 1 < H) {
      //  down center
      sum += (*currentLife)[x + W * (y + 1)];
    }
    if (y - 1 >= 0) {
      // up center
      sum += (*currentLife)[x + W * (y - 1)];
    }
  }
  return sum;
}

void life::update(int n, bool bound) {
  int neighbor;
  // n >0 indicate not use all threads
  if (n > 0) {
    omp_set_num_threads(n);
  }
#pragma omp parallel for
  for (int i = 0; i < size; ++i) {
    neighbor = getNeighbor(i, bound);

    // alive
    if ((*currentLife)[i] == 1) {
      if (neighbor < 2 || neighbor > 3)
        (*nextLife)[i] = 0;
      else
        (*nextLife)[i] = 1;
    } else {
      // dead
      if (neighbor == 3)
        (*nextLife)[i] = 1;
      else
        (*nextLife)[i] = 0;
    }
  }
  std::swap(currentLife, nextLife);
}

void life::read(const std::string& fileName, int e) {
  std::ifstream file(fileName, std::ios_base::in);
  if (file.fail()) {
    throw std::runtime_error("failed to open " + fileName);
  }

  int edge = e;

  std::string line, tmp;

  std::queue<char> charq;

  bool notCounter = true;
  while (std::getline(file, line) && notCounter) {
    tmp = line.substr(0, 2);
    if (tmp == "#C" || tmp == "#c") {
      // comment
      continue;
    } else if (tmp == "#N") {
      // name of pattern
      continue;
    } else if (tmp == "#O") {
      // name of author
      continue;
    } else if (tmp == "#P" || tmp == "#R") {
      //   top-left coordinate
      throw std::runtime_error("not implement R P reading");
    } else if (tmp == "x ") {
      // reading configuration
      std::stringstream ss(line);

      std::regex ruleP("rule = ([\\w\\/]*)");

      auto words_begin = std::sregex_iterator(line.begin(), line.end(), ruleP);
      auto words_end = std::sregex_iterator();

      int index = 0;
      int t[2];

      while (index < 2) {
        ss >> t[index];
        if (ss.fail()) {
          ss.clear();
          ss >> tmp;
        } else {
          index += 1;
        }
      }
      this->W = t[0] + 2 * edge;
      this->H = t[1] + 2 * edge;
      this->size = this->W * this->H;
      // init first edegs and the first line
      for (int i = 0; i < edge * W + edge; i++)
        this->currentLife->push_back(0);

      words_begin = std::sregex_iterator(line.begin(), line.end(), ruleP);
      for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::smatch match = *i;
        // std::cout << (match.str().substr(7)) << std::endl;
        this->rule = match.str().substr(7);
        if (this->rule != "B3/S23" && this->rule != "b3/s23") {
          std::cerr << "rule " + this->rule + " not implement" << std::endl;
          throw std::runtime_error("wrong input file");
        }
      }
    } else {
      // reading
      auto c_str = line.c_str();
      for (size_t i = 0; i < line.size(); i++) {
        if (c_str[i] >= '0' && c_str[i] <= '9') {
          charq.push(c_str[i]);
        } else if (c_str[i] == 'b') {
          // dead
          if (charq.empty()) {
            this->currentLife->push_back(0);
          } else {
            int times = 0;
            while (!charq.empty()) {
              times = 10 * times + (charq.front() - '0');
              charq.pop();
            }
            while (times--) {
              this->currentLife->push_back(0);
            }
          }
        } else if (c_str[i] == 'o') {
          // alive
          if (charq.empty()) {
            this->currentLife->push_back(1);
          } else {
            int times = 0;
            while (!charq.empty()) {
              times = 10 * times + (charq.front() - '0');
              charq.pop();
            }
            while (times--) {
              this->currentLife->push_back(1);
            }
          }
        } else if (c_str[i] == '$') {
          // end of line
          if (charq.empty()) {
            while (this->currentLife->size() % this->W)
              this->currentLife->push_back(0);
            // front edge
            for (int i = 0; i < edge; i++)
              this->currentLife->push_back(0);
          } else {
            int times = 0;
            while (!charq.empty()) {
              times = 10 * times + (charq.front() - '0');
              charq.pop();
            }
            times -= 1;
            while (this->currentLife->size() % this->W)
              this->currentLife->push_back(0);
            while (times--) {
              // front edge
              for (size_t i = 0; i < this->W; i++)
                this->currentLife->push_back(0);
            }
            // front edge
            for (int i = 0; i < edge; i++)
              this->currentLife->push_back(0);
          }
        } else if (c_str[i] == '!') {
          // end of file
          while (this->currentLife->size() != this->H * this->W) {
            this->currentLife->push_back(0);
          }
          break;
        }
      }
    }
  }
  this->nextLife->resize(this->currentLife->size());
}