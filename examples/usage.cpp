#include "FileIO.h"

int main() {
  FileIO<double> f;

  // Read a structured file into a vector
  auto ar = f.ReadFile("multi_col_example.txt", 10, '#');

  // Output vector
  f.PrintArray(ar);

  // Write 2D vector to file
  f.Write2File(ar, "structured_file_writting.txt", "\t");

  // Read a 1D vector from file
  auto one_dim = f.LoadSingleCol("1v_example.txt");
  for (const auto& i : one_dim) std::cout << i << " ";
  std::cout << std::endl;

  // Generate a jagged array
  size_t rows = 10;
  size_t num = 0;
  std::vector<std::vector<double>> jagged(rows);

  for (size_t i = 1; i < rows; ++i) {
    for (size_t j = 0; j < i; j++) {
      jagged[i - 1].push_back(num);
      ++num;
    }
  }

  // Write the jagged array to a file
  f.Write2File(jagged, "jagged_array_write.txt", " ", true );

  // Read jagged array from file
  auto jagged_in = f.ReadFile("jagged_array_write.txt", rows, '#', true);

  // Output jagged array
  f.PrintArray(jagged_in);
}
