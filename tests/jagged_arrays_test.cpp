#include "../FileIO.h"

int main() {
  size_t rows = 10;
  size_t num = 0;
  std::vector<std::vector<double>> jagged(rows);

  // Make the array jagged
  for (size_t i = 1; i < rows; ++i) {
    for (size_t j = 0; j < i; ++j) {
      jagged[i - 1].push_back(num);
      ++num;
    }
  }

  // Write the array to a file
  FileIO f;
  // Get path executable
  std::cout << f.getExecutablePath() << std::endl;
  f.Write2File<double>(jagged, "jagged_array_test.txt", "\t", true);
  // f.PrintArray<double>(jagged);

  // Read the jagged array intoa vector
  auto in = f.ReadFile<double>("jagged_array_test.txt", rows, '#', true);
  // f.PrintArray<double>(in);

  if (jagged == in) {
    std::cout << "YES\tRead/Write of jagged arrays passes" << std::endl;
  } else {
    std::cout << "NO\tRead/Write of jagged arrays fails" << std::endl;
  }
}