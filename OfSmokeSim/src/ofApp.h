#pragma once

#include "ofMain.h"
#include "SmokeSolver.hpp"
#include "BoundaryBox.hpp"
#include "SmokeDataLoader.hpp"

#include <string>

/*
	To create a mp4 file from screenshots: 
		- change saveFrames to true
		- ffmpeg -framerate x -start_number 0 -i path/to/frames/frame%d.png path/to/vid/videoName.mp4

	Camera rotation speed: 'a' to accelerate towards left, 'd' to accelerate towards right by 10 degrees per second
	Camera vertical movement: 'w' to move the camera up by 10 degrees, 's' to move the camera down by 10 degrees
	Camera proximity: 'q' to move the camera farther from the box, 'e' to move the camera closer to the box
	Camera pause: 'p' to pause camera 
*/
class ofApp : public ofBaseApp{

	public:
		ofApp(const std::string &config_filename)
			: smoke_data_loader(SmokeDataLoader(config_filename)) {}

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		void cameraControl();
		void drawSmoke(Field3D &field);
		string createFrameDirectory();

		ofEasyCam cam;
		ofShader shader;
		bool pauseRotation = false;
		float cameraOrbitLongitudeDPS = 30.0f;
		float cameraOrbitLatitude = -10.0f;
		float cameraOrbitRadius;
		float currentCameraLongitude = 0.0f;
	
	private:
		SmokeDataLoader smoke_data_loader;
};
