#include "FileIO.h"

int main() {
  FileIO<double> f;
  f.PrintFile("example.txt", 2, '#');
}

