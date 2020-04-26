# FileIO: Easy to use file-reading/writting template

[![Build Status](https://travis-ci.com/GNikit/FileIO.svg?branch=master)](https://travis-ci.com/GNikit/FileIO)

## Description

A simple header only implementation read/write template class for:

1. Writting 2D and 1D vectors in ASCII to file
2. Reading whole or partial (certain columns) ASCII files into 2D and 1D vectors
3. Displaying 2D and 1D vectors

The class offers support for writing vectors to files in by using
one of two formats:

- Row major (each vector is a row in the output file)
- Column major (each vector is a column in the output file)

Similar functionality to `numpy.loadtxt()`.
Returns a `vector<vector<T>>` of the read columns.
In the case where the user-defined columns *n* are less than the file columns,
`ReadFile()` will read only the first *n* columns.

## Usage

Sample code reading and writing a 2D vector

```C++
#include "FileIO.h"

int main() {

  FileIO file;
  // Load 2D vector from file
  // args: filename, file columns, comment char, format=1:column-major
  std::vector<std::vector<double>> test = file.ReadFile<double>("example.log", 2, '#');
  // Write 2D vector to file
  // args: 2D vector, filename, delimiter
  file.Write2File<double>(test, "example_copy.log", " ");
}
```
