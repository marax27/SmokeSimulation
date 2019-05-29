#include <iostream>
#include <sstream>
#include <fstream>
#include "SmokeSolver.hpp"
#include <map> 

class SmokeDataLoader {

	std::map<std::string, double> parameters;

public:

	SmokeDataLoader(){
		std::ifstream is;
		is.open("parameters.cfg");
		// get length of file:
		is.seekg(0, std::ios::end);
		long length = is.tellg();
		is.seekg(0, std::ios::beg);

		char* buffer = new char[length];
		is.read(buffer, length);

		std::istringstream is_file(buffer);
		std::string line;
		while (std::getline(is_file, line))
		{
			std::istringstream is_line(line);
			std::string key;
			if (std::getline(is_line, key, '='))
			{
				std::string value;
				if (std::getline(is_line, value))
					parameters.insert(std::pair<std::string,double>( key, std::stod(value) ));
			}
		}
		delete[] buffer;
		is.close();
	}
	
	void loadData(SmokeSolver& smokeSolver) {
		smokeSolver.setDt(parameters.find("dt")->second);
		smokeSolver.setDx(parameters.find("dx")->second);
		smokeSolver.setKinematicViscosity(parameters.find("kv")->second);
		smokeSolver.setFluidDensity(parameters.find("fd")->second);
		smokeSolver.setFallCoefficient(parameters.find("fc")->second);
		smokeSolver.setRiseCoefficient(parameters.find("rc")->second);
		smokeSolver.setSmokeDiffusionCoefficient(parameters.find("sdc")->second);
		smokeSolver.setDensityThreshold(parameters.find("dtr")->second);
		smokeSolver.setVorticityConfinementCoefficient(parameters.find("vcc")->second);
		smokeSolver.setDensityDecay(parameters.find("dd")->second);
		smokeSolver.setWindCoefficient(parameters.find("wc")->second);
		double wvX = parameters.find("wvX")->second;
		double wvY = parameters.find("wvY")->second;
		double wvZ = parameters.find("wvZ")->second;
		smokeSolver.setWindVelocity({ wvX, wvY, wvZ });
	}

	void printParams() {
		std::map<std::string, double>::iterator itr;
		std::cout << "\tKEY\tELEMENT\n";
		for (itr = parameters.begin(); itr != parameters.end(); ++itr) {
			std::cout << '\t' << itr->first
				<< '\t' << itr->second << '\n';
		}
	}

};