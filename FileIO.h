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
  // TODO: make it return a tuple of vectors
  // could be done by either templating a second template variable TUPLE_N
  // TUPLE_N FileIO<T>::ReadFile(const std::string & file_name, blah
  // blah blah) or using a vardic template
  // http://aherrmann.github.io/programming/2016/02/28/unpacking-tuples-in-cpp14/

  /**
   * @brief
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
   * @tparam T double, integer, size_t, etc.
   * @param file_name name + relative path to the file including the extension
   * @param columns The total number of columns in the file
   * @param comment Character to be treated as comment. Lines starting with
   *                comment will be ignored
   * @param format Parameter which indicates the format of the file
   *               i.e. what each sub-vector correspond to in terms of the
   *               original file
   *               - 0: file rows loaded as sub-vectors
   *               - 1: file columns loaded as sub-vectors
   *               - 2: jagged (ends each line with a delimeter character)
   * @return std::vector<std::vector<T>> Data, where every file column is a
   *                                     vector row
   */
  template <typename T>
  static std::vector<std::vector<T>> ReadFile(const std::string& file_name,
                                              size_t columns,
                                              char comment = '#',
                                              size_t format = 0);

  /**
   * @brief
   * Prints the file contents to the terminal, without the comments or headers.
   * The method assumes that all columns are of the same length.
   *
   * @tparam T double, integer, size_t, etc.
   * @param file_name name + relative path to the file including the extension
   * @param columns The total number of columns in the file
   * @param comment Character to be treated as comment. Lines starting with
   *                comment will be ignored
   * @return std::vector<std::vector<T>> Data, where every file column is a
   *                                     vector row
   */
  template <typename T>
  static std::vector<std::vector<T>> PrintFile(std::string const& file_name,
                                               size_t columns,
                                               char comment = '#');

  /**
   * @brief Write a 2D vector to a file.
   *
   * @tparam T double, integer, size_t, etc.
   * @param data 2D data vector
   * @param file_name name + relative path to the file including the extension
   * @param del The delimiter that the file will be separated with
   * @param header: An optional header for the file
   * @param format Parameter which indicates the format of the file
   *               i.e. what each sub-vector correspond to in terms of the
   *               original file
   *               - 0: file rows loaded as sub-vectors
   *               - 1: file columns loaded as sub-vectors
   *               - 2: jagged (ends each line with a delimeter character)
   */
  template <typename T>
  static void Write2File(std::vector<std::vector<T>>& data,
                         std::string const& file_name, std::string const& del,
                         std::string const& header = "", size_t format = 0,
                         bool index = false);

  /**
   * @brief Write a 2D vector to a file.
   *
   * @tparam T double, integer, size_t, etc.
   * @param data 2D data vector
   * @param f filestream to be used for
   * @param del The delimiter that the file will be separated with
   * @param header: An optional header for the file
   * @param format Parameter which indicates the format of the file
   *               i.e. what each sub-vector correspond to in terms of the
   *               original file
   *               - 0: file rows loaded as sub-vectors
   *               - 1: file columns loaded as sub-vectors
   *               - 2: jagged (ends each line with a delimeter character)
   */
  template <typename T>
  static void Write2File(std::vector<std::vector<T>>& data, std::ofstream& f,
                         std::string const& del, std::string const& header = "",
                         size_t format = 0, bool index = false);

  template <typename T>
  static void Write2File(std::vector<std::vector<T>*>& data, std::ofstream& f,
                         std::string const& del, std::string const& header = "",
                         size_t format = 0, bool index = false);

  /**
   * @brief Takes a 1D vector as input and writes it to a file regardless of its
   * structure.
   *
   * @tparam T double, integer, size_t, etc.
   * @param data: 1D data structure
   * @param file_name: The name/relative path to the file (including the
   * extension)
   */
  template <typename T>
  static void Write2File(std::vector<T>& data, const std::string& file_name);

  template <typename T>
  static void PrintArray(std::vector<std::vector<T>>& data);

  /**
   * @brief
   *  Reads a file that is structured with data in a column.
   *  No option for comments or headers.
   */
  template <typename T>
  static std::vector<T> LoadSingleCol(const std::string& file_name);

  // TODO: need to find a way to move this from preprocessor into the class
  static std::string getExecutablePath();

  /**
   * @brief Performs a find a replace on a string
   *
   * @param source String to operate on
   * @param find Find in string
   * @param replace String to replace with
   * @return std::string Replaced string
   */
  static std::string find_and_replace(std::string& source,
                                      const std::string& find,
                                      const std::string& replace);

 protected:
  static void get_time(std::ofstream& stream);
};

template <typename T>
std::vector<std::vector<T>> FileIO::ReadFile(std::string const& file_name,
                                             size_t columns, char comment,
                                             size_t format) {
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

    // Allows reading of row ordered arrays OR jagged arrays
    // In the case of jagged arrays the sub-arrays are row-major
    // hence every line in the file corresponds to a sub-array
    if (format == 0 || format == 2) {
      size_t row = 0;
      while (std::getline(file, line)) {
        if (line.size() != 0 && line[0] != comment) {
          // Pass string to input string stream
          std::istringstream ss(line);

          // Read the numbers into a temp vector
          std::vector<T> t;
          t.reserve(RESERVE_MEMORY);
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
                        const std::string& filename, const std::string& del,
                        const std::string& header, size_t format, bool index) {
  std::ofstream f;
  f.open(filename, std::ios::out | std::ios::trunc);

  Write2File<T>(data, f, del, header, format, index);
}

template <typename T>
void FileIO::Write2File(std::vector<std::vector<T>>& data, std::ofstream& f,
                        const std::string& del, const std::string& header,
                        size_t format, bool index) {
  if (!header.empty()) f << header << std::endl;

  /**
   * Switch that determines the formating of the file, how each sub-vector will
   * be output.
   * The preset option, 0, will place each sub-vector as a row of the file and
   * will not include a the selected delimiter at the end of a line (EOL).
   * This is the only difference between formats 0 and 2.
   * Otherwise 2 is way more elegant and a couple % faster.
   *
   * format 1 is to be used when wishing to output the array with a column
   * ordering effectively transposing it. A bit slower than both other formats.
   */
  if (format == 0) {
    size_t row, col;
    for (row = 0; row < data.size(); ++row) {
      if (index) f << (row + 1) << del;
      // Write all but the last entry, since we want the last entry to not
      // have both a del character and an EOL character
      for (col = 0; col < data[row].size() - 1; ++col) {
        f << data[row][col] << del;
      }
      f << data[row][col] << std::endl;
    }
  } else if (format == 1) {
    size_t row, col;
    for (col = 0; col < data[0].size(); ++col) {
      if (index) f << (col + 1) << del;
      // Write all but the last entry, since we want the last entry to not
      // have both a del character and an EOL character
      for (row = 0; row < data.size() - 1; ++row) {
        f << data[row][col] << del;
      }
      f << data[row][col] << std::endl;
    }
  } else if (format == 2) {
    size_t i = 1;
    for (const auto& row : data) {
      if (index) f << i << del;
      for (const auto& col : row) {
        f << col << del;  // todo: adds del + EOL, fix!
        ++i;
      }
      f << std::endl;
    }
  } else {
    std::cerr << "Supplied format option: " + std::to_string(format)
              << " not supported/implemented. Exiting without writing anything!"
              << std::endl;
  }

  f.close();
}

template <typename T>
void FileIO::Write2File(std::vector<std::vector<T>*>& data, std::ofstream& f,
                        const std::string& del, const std::string& header,
                        size_t format, bool index) {
  if (!header.empty()) f << header << std::endl;

  /**
   * Switch that determines the formating of the file, how each sub-vector will
   * be output.
   * The preset option, 0, will place each sub-vector as a row of the file and
   * will not include a the selected delimiter at the end of a line (EOL).
   * This is the only difference between formats 0 and 2.
   * Otherwise 2 is way more elegant and a couple % faster.
   *
   * format 1 is to be used when wishing to output the array with a column
   * ordering effectively transposing it. A bit slower than both other formats.
   */
  if (format == 0) {
    size_t row, col;
    for (row = 0; row < data.size(); ++row) {
      if (index) f << (row + 1) << del;
      // Write all but the last entry, since we want the last entry to not
      // have both a del character and an EOL character
      for (col = 0; col < data[row]->size() - 1; ++col) {
        f << data[row]->at(col) << del;
      }
      f << data[row]->at(col) << std::endl;
    }
  } else if (format == 1) {
    size_t row, col;
    for (col = 0; col < data[0]->size(); ++col) {
      if (index) f << (col + 1) << del;
      // Write all but the last entry, since we want the last entry to not
      // have both a del character and an EOL character
      for (row = 0; row < data.size() - 1; ++row) {
        f << data[row]->at(col) << del;
      }
      f << data[row]->at(col) << std::endl;
    }
  } else if (format == 2) {
    // for (const auto& row : data) {
    //   for (const auto& col : row) {
    //     f << col << del;  // todo: adds del + EOL, fix!
    //   }
    //   f << std::endl;
    // }
  } else {
    std::cerr << "Supplied format option: " + std::to_string(format)
              << " not supported/implemented. Exiting without writing anything!"
              << std::endl;
  }

  f.close();
}

template <typename T>
void FileIO::Write2File(std::vector<T>& data, const std::string& file_name) {
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

std::string FileIO::find_and_replace(std::string& source,
                                     const std::string& find,
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
  stream << "# Created on: " << std::ctime(&date_time);
}
