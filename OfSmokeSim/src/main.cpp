#include "ofMain.h"
#include "ofApp.h"

#include <iostream>
#include <sstream>

template<typename T> T convertTo(const std::string &s){
	T result;
	std::stringstream ss{s};
	ss >> result;
	return result;
}

//========================================================================
int main(int argc, char **argv){
	int x = 30, y = 30, z = 30;

	if(argc == 5){
		x = convertTo<int>(argv[2]);
		y = convertTo<int>(argv[3]);
		z = convertTo<int>(argv[4]);
	}else if(argc != 2 || std::string(argv[1]) == "--help"){
		std::cout << "Usage: " << argv[0] << " config-filename (X Y Z)\n";
		return 1;
	}

	ofGLFWWindowSettings settings;
	settings.setGLVersion(3, 2);
	settings.setSize(1280, 720);
	ofCreateWindow(settings);

	try{
		ofRunApp(new ofApp(argv[1], x, y, z));
	}catch(std::runtime_error exc){
		std::cerr << "[ERROR] " << exc.what() << '\n';
		return 1;
	}
}
