#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#ifdef _WIN32
#include <windows.h>
std::string getExePath() {
  char result[MAX_PATH];
  return std::string(result, GetModuleFileName(NULL, result, MAX_PATH));
}
#else
#include <linux/limits.h>
#include <unistd.h>
std::string getExePath() {
  char result[PATH_MAX];
  ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
  return std::string(result, (count > 0) ? count : 0);
}
#endif

#define RESERVE_MEMORY 100000

class FileIO {
 public:
  FileIO();
  ~FileIO();

  // TODO: make it return a tuple of vectors
  // could be done by either templating a second template variable TUPLE_N
  // ---> TUPLE_N FileIO<T>::ReadFile(const std::string & file_name, blah
  // blah blah) or using a vardic template
  //--->http://aherrmann.github.io/programming/2016/02/28/unpacking-tuples-in-cpp14/
  // TODO: Investigate why vec2d not working outside of class def in template
  // ReadFile
  template <typename T>
  std::vector<std::vector<T>> ReadFile(const std::string& file_name,
                                       size_t columns, char comment = '#',
                                       bool jagged = false);

  template <typename T>
  std::vector<std::vector<T>> PrintFile(const std::string& file_name,
                                        size_t columns, char comment = '#');

  template <typename T>
  void Write2File(std::vector<std::vector<T>>& data,
                  const std::string& file_name,
                  const std::string& del,
                  bool jagged = false);

  template <typename T>
  void Write2File(std::vector<T>& data,
                  const std::string& file_name);

  template <typename T>
  void PrintArray(std::vector<std::vector<T>>& data);

  template <typename T>
  std::vector<T> LoadSingleCol(const std::string& file_name);

  // TODO: need to find a way to move this from preprocessor into the class
  std::string getExecutablePath();

  std::string find_and_replace(std::string& source, const std::string& find,
                               const std::string& replace);

 protected:
  void get_time(std::ofstream& stream);
};

FileIO::FileIO() {}

FileIO::~FileIO() {}

template <typename T>
std::vector<std::vector<T>> FileIO::ReadFile(const std::string& file_name,
                                             size_t columns, char comment,
                                             bool jagged) {
  /*
   *  Used to read a file with either "space" or "tab" delimated columns.
   *  The number of columns in the file has to be known and be passed as an
   *  argument.
   *  
   *  --structured arrays--
   *      |0 1 2|                                |0 3 6|
   *  v = |3 4 5| ; then v[0] = [0 1 2] ; file = |1 4 7|
   *      |6 7 8|                                |2 5 8|  
   *  
   *  --jagged arrays--
   *      |0    |                            |0    |
   *  v = |1 2  | ; then v[0] = [0] ; file = |1 2  |
   *      |3 4 5|                            |3 4 5|
   * 
   *  In the case where the array is jagged, then no transpose occurs 
   *  and the vector is written in the file as-is.
   *
   *  In the case where a smaller number of columns is supplied, than the number
   *  contained in the file, then only these columns will be extracted.
   *
   *  @param file_name: The name/relative path to the file with extension
   *  @param columns: The total number of columns in the file
   *  @param comment: Character to be treated as comment. Lines starting with
   *                  comment will be ignored
   *  @return data[][]: Vector of vectors, with each sub-vector being a read
   *                    column
   */

  std::ifstream file(file_name);
  file.exceptions(file.failbit);  // Throws exception
  std::vector<std::vector<T>> data;

  try {
    T temp;
    data.resize(columns);
    std::string line;

    // Reserve space for column vectors
    for (size_t i = 0; i < data.size(); ++i) {
      data[i].reserve(RESERVE_MEMORY);
    }

    // Allows reading of jagged arrays
    // In the case of jagged arrays the sub-arrays are row-major
    // hence every line in the file corresponds to a sub-array
    if (jagged) {
      size_t row = 0;
      while (std::getline(file, line)) {
        if (line.size() != 0 && line[0] != comment) {
          // Pass string to input string stream
          std::istringstream ss(line);

          // Read the nubers into a temp vector
          std::vector<T> t;  // Possibly reserve some space more efficient
          T num;
          // Pass values from string stream to vector
          while (ss >> num) {
            t.push_back(num);
          }

          // Appending elements to the 2D array
          data[row].insert(std::end(data[row]), std::begin(t), std::end(t));
          ++row;
        }
      }
    }
    // If the array is structured the columns of the file
    // correspond to the sub-arrays in the data vector
    else {
      while (std::getline(file, line)) {
        if (line.size() != 0 && line[0] != comment) {
          std::stringstream ss(line);

          // could include testing for tabs == columns -1
          // otherwise throw range_error or logic_error exception
          // catch that const std::exception &e
          for (size_t i = 0; i < columns; ++i) {
            ss >> temp;
            data[i].push_back(temp);
          }
        }
      }
    }
    file.close();  // just in case it fails to close
  } catch (const std::ios_base::failure& e) {
    if (!file.eof())
      /*if EOF sets eofbit/failbit 0, normally getline sets to 1
                        if nothing is extracted from a line. */
      std::cerr << "Caught an ios_base::failure.\n"
                << "Explanatory string: " << e.what() << '\n'
                << "Error code: " << e.code() << '\n';
  }
  return data;
}

template <typename T>
std::vector<std::vector<T>> FileIO::PrintFile(const std::string& file_name,
                                              size_t columns, char comment) {
  FileIO f;
  /*
   * Prints the file contents to the terminal, without the comments or headers.
   * The method assumes that all columns are of the same length.
   * 
   *  @param file_name: The name/relative path to the file with extension
   *  @param columns: The total number of columns in the file
   *  @param comment: Character to be treated as comment. Lines starting with
   *                  comment will be ignored
   *  @return data[][]: Vector of vectors, with each sub-vector being a read
   *                    column
   */

  // File Contents vector
  std::vector<std::vector<T>> fc;

  // Read file into fc
  fc = f.ReadFile<T>(file_name, columns, comment);

  // Prints only the file contents not the comments of the file
  size_t col = 0;
  while (col < fc[0].size()) {
    for (size_t i = 0; i < fc.size(); ++i) {
      std::cout << fc[i][col] << '\t';
    }
    std::cout << std::endl;
    ++col;
  }
  return fc;
}

template <typename T>
void FileIO::Write2File(std::vector<std::vector<T>>& data,
                        const std::string& file_name,
                        const std::string& del,
                        bool jagged) {
  /*
   * THIS METHOD TRANPOSES THE VECTOR IF THE ARRAY IS NOT JAGGED!!!
   * e.g. a [10][2] will be written as a [2][10]
   * Takes a 2D vector as input and writes it to a file 
   * regardless of its structure.
   * 
   * @param data: 2D data structure
   * @param file_name: The name/relative path to the file with extension
   * @param del: The delimiter that the file will be separated with
   */

  // TODO: currently only handles 2D vectors can't be bothered to make it more general
  // can be generalised by making it a T,U template with U the typw of 'data' variable
  // Open stream out
  std::ofstream f;
  f.open(file_name, std::ios::out | std::ios::trunc);

  if (jagged) {
    for (const auto& row : data) {
      for (const auto& col : row) {
        f << col << del;
      }
      // For some reason this executes twice at EOF
      f << std::endl;
    }
  } else {
    for (size_t col = 0; col < data[0].size(); ++col) {
      for (size_t i = 0; i < data.size(); ++i) {
        f << data[i][col] << del;
      }
      f << std::endl;
    }
  }
  f.close();
}

template <typename T>
void FileIO::Write2File(std::vector<T>& data,
                        const std::string& file_name) {
  /*
   * Takes a 1D vector as input and writes it to a file 
   * regardless of its structure.
   * 
   * @param data: 1D data structure
   * @param file_name: The name/relative path to the file with extension
   * @param del: The delimiter that the file will be separated with
   */

  // TODO: currently only handles 2D vectors can't be bothered to make it more general
  // can be generalised by making it a T,U template with U the typw of 'data' variable
  // Open stream out
  std::ofstream f;
  f.open(file_name, std::ios::out | std::ios::trunc);

  for (const auto& row : data) {
    f << row << std::endl;
  }
  f.close();
}

template <typename T>
void FileIO::PrintArray(std::vector<std::vector<T>>& data) {
  for (const auto& i : data) {
    for (const auto& j : i) {
      std::cout << j << '\t';
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

template <typename T>
std::vector<T> FileIO::LoadSingleCol(const std::string& file_name) {
  /*
   *  Reads a file that is structured with data in a column.
   *  No option for comments or headers.
   */
  std::vector<T> data;
  data.reserve(RESERVE_MEMORY);  // increases performance for large files
  std::ifstream file(file_name);
  file.exceptions(file.failbit);  // Throws exception

  try {
    std::copy(std::istream_iterator<T>(file), std::istream_iterator<T>(),
              std::back_inserter(data));
  }

  catch (const std::ios_base::failure& e) {
    if (!file.eof())
      /*if EOF sets eofbit/failbit 0, normally getline sets to 1
                        if nothing is extracted from a line. */
      std::cerr << "Caught an ios_base::failure.\n"
                << "Explanatory string: " << e.what() << '\n'
                << "Error code: " << e.code() << '\n';
  }
  return data;
}

std::string FileIO::getExecutablePath() {
  // TODO: find a better implementation without redefinition inside the template
  std::string dir = getExePath();
#ifdef _WIN32
    dir = find_and_replace(dir, "\\", "/");
#endif
  return dir;
}

std::string FileIO::find_and_replace(std::string& source, const std::string& find,
                                     const std::string& replace) {
  for (std::string::size_type i = 0;
       (i = source.find(find, i)) != std::string::npos;) {
    source.replace(i, find.length(), replace);
    i += replace.length();
  }
  return source;
}

void FileIO::get_time(std::ofstream& stream) {
  std::chrono::time_point<std::chrono::system_clock> instance;
  instance = std::chrono::system_clock::now();
  std::time_t date_time = std::chrono::system_clock::to_time_t(instance);
  stream << "#Created on: " << std::ctime(&date_time);
}
