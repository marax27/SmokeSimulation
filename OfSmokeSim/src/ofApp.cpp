#include "ofApp.h"

//--------------------------------------------------------------
int x = 30, y = 30, z = 30;
double dx = 0.5;
SmokeSolver smokeSolver(x, y, z);
BoundaryBox boundaryBox(x * dx, y * dx, z * dx);

void ofApp::setup() {
	ofSetFrameRate(4);
	ofEnableDepthTest();
	ofBackground(0, 0, 0);

	smokeSolver.setBuoyancy(0.3);
	smokeSolver.setDt(0.01);
	smokeSolver.setDx(dx);
	smokeSolver.setKinematicViscosity(0.001);

	cam.disableMouseInput();

	cout << "START" << endl;
}

//--------------------------------------------------------------
void ofApp::update(){
	smokeSolver.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	auto field = smokeSolver.getDensityField();
	double currentHighestDensity = highestDensity;
	double density, alpha;

	cam.begin();
	cameraControl();

	boundaryBox.draw();

	for (int i = 1; i < field.XLast() - 1; ++i) {
		for (int j = 1; j < field.YLast() - 1; ++j) {
			for (int k = 1; k < field.ZLast() - 1; ++k) {
				density = field(i, j, k);
				if (density != 0) {
					highestDensity = highestDensity < density ? density : highestDensity;
					alpha = setParticleAlpha(density, currentHighestDensity);
					p.draw(i, j, k, dx, alpha);
				}
			}
		}
	}
	
	cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key == 'p')
		pauseRotation = !pauseRotation;
	if(key == 'w')
		cameraOrbitLatitude -= 10.0f;
	if(key == 's')
		cameraOrbitLatitude += 10.0f;
	if(key == 'd')
		cameraOrbitLongitude += 5.0f;
	if(key == 'a')
		cameraOrbitLongitude -= 5.0f;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

double ofApp::setParticleAlpha(double density, double highestDensity) {
	return (density * 255) / highestDensity;
}

void ofApp::cameraControl() {
	cam.setTarget(ofPoint(dx * x / 2, dx * y / 2, dx * z / 2));

	if(!pauseRotation) {
		cam.orbitDeg(cameraOrbitLongitude * ofGetElapsedTimef(), 
					cameraOrbitLatitude, 
					30, 
					ofPoint(dx * x / 2, dx * y / 2, dx * z / 2));
					cout << cameraOrbitLongitude * ofGetElapsedTimef() << " " << cameraOrbitLatitude << endl;
	}
}