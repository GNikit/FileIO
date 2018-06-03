#include "FileLoading.h"

int main() {

	//FileLoading<double>* file = new FileLoading<double>;
	FileLoading<double> file;
	std::string file_path = "C:/Code/C++/MD simulation/Archives of Data/Data_step_10000_particles_1000_rho_0.5000_T_0.5000_n_6_A_0.00000.txt";
	std::vector<std::vector<double>> test = file.LoadTxt(file_path, 2, '#');
	std::vector<double> column0 = test[0];
	std::vector<double> column1 = test[1];
}
