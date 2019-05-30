#include "ofMain.h"
#include "ofApp.h"

#include <iostream>

//========================================================================
int main(int argc, char **argv){
	if(argc != 2 || std::string(argv[1]) == "--help"){
		std::cout << "Usage: " << argv[0] << " config-filename\n";
		return 1;
	}

	ofGLFWWindowSettings settings;
	settings.setGLVersion(3, 2);
	settings.setSize(1280, 720);
	ofCreateWindow(settings);

	try{
		ofRunApp(new ofApp(argv[1]));
	}catch(std::runtime_error exc){
		std::cerr << "[ERROR] " << exc.what() << '\n';
		return 1;
	}
}
