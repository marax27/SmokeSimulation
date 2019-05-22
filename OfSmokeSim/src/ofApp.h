#pragma once

#include "ofMain.h"
#include "SmokeSolver.hpp"
#include "Particle.h"
#include "BoundaryBox.hpp"

class ofApp : public ofBaseApp{

	public:
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
		double setParticleAlpha(double density, double highestDensity);

		ofEasyCam cam;
		//SmokeSolver ss;
		Particle p;
		//double dx;
		double highestDensity = 0;
		bool pauseRotation = false;
		float cameraOrbitLongitude = 10.0f;
		float cameraOrbitLatitude = -10.0f;
};
