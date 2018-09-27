#include "../FileIO.h"

int main() {
  // Generate vector, write to file, read it and compare equal

  size_t rows = 10;
  size_t cols = 3;
  size_t num = 0;
  std::vector<std::vector<double>> data(rows);

  // Populate the vector
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      data[i].push_back(num);
      ++num;
    }
  }
  FileIO<double> f;
  // Write data to file
  f.Write2File(data, "2d_vector_write_test.txt", "\t", false);

  // Read data from file
  auto in = f.ReadFile("2d_vector_write_test.txt", rows, '#', false);

  if (data == in) {
    std::cout << "YES\tRead/Write of structured arrays passes" << std::endl;
  } else {
    std::cout << "NO\tRead/Write of structured arrays fails" << std::endl;
  }
}