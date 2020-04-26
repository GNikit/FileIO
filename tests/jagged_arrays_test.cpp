#include <assert.h>
#include "../FileIO.h"

int main() {
  size_t rows = 10;
  size_t num = 0;
  std::vector<std::vector<double>> jagged(rows);

  // Make the array jagged
  for (size_t i = 1; i < jagged.size() + 1; ++i) {
    for (size_t j = 0; j < i; ++j) {
      jagged[i - 1].push_back(num);
      ++num;
    }
  }

  // Write the array to a file
  FileIO f;
  f.Write2File<double>(jagged, "jagged_array_test.txt", "\t", "", 0);
  // f.PrintArray<double>(jagged);

  // Read the jagged array into a vector
  auto in = f.ReadFile<double>("jagged_array_test.txt", rows, '#', 0);
  // f.PrintArray<double>(in);

  if (jagged == in) {
    std::cout << "YES\tRead/Write of jagged arrays passes" << std::endl;
    assert(true);
  } else {
    std::cout << "NO\tRead/Write of jagged arrays fails" << std::endl;
    assert(false);
  }
}