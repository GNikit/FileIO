#include "FileLoading.h"



FileLoading::FileLoading() {
}


FileLoading::~FileLoading() {
}


std::vector<std::vector<double>> FileLoading::LoadTxt(const std::string &file_name,
                                                      size_t columns, char comment) {
  try {
    std::ifstream read_file(file_name);
    read_file.exceptions(read_file.failbit);  // Throws exception
    double temp;

    data.resize(columns);

    for (int i = 0; i < data.size(); i++) {
      // Reserve space for column vectors
      data.at(i).reserve(50000);
    }

    std::string line;
    while (!read_file.eof()) {  // Stops when End Of File is reached
      std::getline(read_file, line, '\t');

      if ((line[0] != comment) && (line.length() != 0)) {
        std::stringstream ss(line);

        for (int i = 0; i < columns; i++) {
          ss >> temp;
          data[i].push_back(temp);
        }
      }
    }
    read_file.close();
    return data;
  }
  catch (const std::ios_base::failure& e) {
    std::cout << "Caught an ios_base::failure.\n"
      << "Explanatory string: " << e.what() << '\n'
      << "Error code: " << e.code() << '\n';
  }



}