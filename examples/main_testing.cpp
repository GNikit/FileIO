#include "FileIO.h"

int main() {
  FileIO<double> f;
  // Testing PrintFile on structured arrays
  std::cout << "PrintFile structured test" << std::endl;
  f.PrintFile("multi_col_example.txt", 10, '#');

  // Testing ReadFile on structured arrays
  std::cout << "ReadFile structured test" << std::endl;
  auto ar = f.ReadFile("multi_col_example.txt", 10, '#');

  for (const auto& i : ar[0]) {
    std::cout << i << '\t';
  }
  std::cout << std::endl;

  // Write2File
  f.Write2File(ar, "write2file_test.txt", "\t", false);
  std::cout << "\nOutputs should be identical\n"
            << std::endl;
  f.PrintFile("write2file_test.txt", 10, '#');

  // Testing the multicolumn, irregular, file writting
  std::cout << "Write2File unstructured test" << std::endl;
  std::vector<std::vector<double>> a(10);
  // Create an irregular 2D vector
  for (int i = 1; i < 10; ++i) {
    for (int j = 0; j < i; ++j) {
      a[i].push_back(j);
    }
  }
  // // TODO: current implementation broke this feature
  // // f.Write2File(a, "irregular_vec_to_file.txt", " ");

  // // Load the saved file
  // std::cout << "ReadFile unstructured test" << std::endl;
  // // TODO: ReadFile fails to read unstructed files
  // auto b = f.ReadFile("irregular_vec_to_file.txt", 10, '#');
  // f.Write2File(b, "irregular_file_read_test.txt", " ");

  // // Write a single vector to file
}
