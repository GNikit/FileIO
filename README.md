# FileLoading: Easy to use file-reading template


## Description
A template that allows to read multiple columns of a tab or space delimimated file with ease. 
Similar functionality to `numpy.loadtxt()`.
Returns a `vector<vector<T>>` of the read columns.
In the case where the user-defined columns *n* are less than the file columns, `LoadTxt()` will read only the first *n* columns.


## Usage
Code to run the program can be found in **Source.cpp**. The example above, demonstrates the basic idea.

```
#include "FileLoading.cpp"

int main() {

        FileLoading<double> file;
	std::vector<std::vector<double>> test = file.LoadTxt(example.txt", 2, '#'); // args: filename, ncols, commentchar
	std::vector<double> column0 = test[0];
	std::vector<double> column1 = test[1];
}
```


