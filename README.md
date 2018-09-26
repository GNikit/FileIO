# FileIO: Easy to use file-reading/writting template


## Description
A template that allows to read multiple columns of a tab or space delimimated file with ease. 
Similar functionality to `numpy.loadtxt()`.
Returns a `vector<vector<T>>` of the read columns.
In the case where the user-defined columns *n* are less than the file columns, `ReadFile()` will read only the first *n* columns.


## Usage
Reading a two-column file.
```
#include "FileIO.h"

int main() {

        FileIO<double> file;
	// Load 2D vector from file
	std::vector<std::vector<double>> test = file.ReadFile("example.txt", 2, '#'); // args: filename, ncols, commentchar
	// Write 2D vector to file
	file.Write2File(test, "example_copy.txt", " "); // args: 2D vector, filename, delimiter
}
```


