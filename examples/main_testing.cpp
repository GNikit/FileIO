#include "FileIO.h"

int main() {
  FileIO<double> f;

  std::vector<std::vector<double>> fc;
  fc = f.ReadFile("example.txt", 2, '#');

  size_t col = 0;
  while (col < fc[0].size()) {
    for (size_t i = 0; i < fc.size(); i++) {
      std::cout << fc[i][col] << ' ';
    }
    std::cout << std::endl;
    ++col;
  }
}

