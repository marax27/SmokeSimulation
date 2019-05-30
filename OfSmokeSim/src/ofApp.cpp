#include "ofApp.h"
#include "SmokeDataLoader.hpp"

//--------------------------------------------------------------
int x = 30, y = 30, z = 30;
int fps = 64;
double dx = .5;

bool saveFrames = false;
string framesDirPath = "";

SmokeSolver smokeSolver(x, y, z);
BoundaryBox boundaryBox(x * dx, y * dx, z * dx);

void ofApp::setup() {
	ofSetFrameRate(fps);
	ofBackground(0, 0, 0);

	smoke_data_loader.loadData(smokeSolver);

	cam.disableMouseInput();
	shader.load("smokeShader");
	
	if(saveFrames)
		framesDirPath = createFrameDirectory();

	cameraOrbitRadius = (x + y + z) / 3;

	cout << "START" << endl;
}

//--------------------------------------------------------------
void ofApp::update(){
	smokeSolver.update();

	stringstream windowTitleStream;
	windowTitleStream << "Smoke sim\tFPS " << fixed << setprecision(2) << ofGetFrameRate();
	windowTitleStream << " | delta-t < " << smokeSolver.getThresholdDt(true);
	ofSetWindowTitle(windowTitleStream.str());
}

//--------------------------------------------------------------
void ofApp::draw() {
	auto field = smokeSolver.getDensityField();
	
	cam.begin();

	cameraControl();

	boundaryBox.draw();
	
	shader.begin();
	drawSmoke(field);
	shader.end();

	cam.end();

	if(!framesDirPath.empty()) {
		ofSaveScreen(framesDirPath + "/" + "frame" + to_string(ofGetFrameNum()) + ".png");
	}
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
		cameraOrbitLongitudeDPS += 10.0f;
	if(key == 'a')
		cameraOrbitLongitudeDPS -= 10.0f;
	if(key == 'q')
		cameraOrbitRadius += 5.0f;
	if(key == 'e')
		cameraOrbitRadius -= 5.0f;
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

void ofApp::cameraControl() {
	cam.setTarget(ofPoint(dx * x / 2, dx * y / 2, dx * z / 2));

	if(!pauseRotation) {
		cam.orbitDeg(currentCameraLongitude, 
					cameraOrbitLatitude, 
					cameraOrbitRadius, 
					ofPoint(dx * x / 2, dx * y / 2, dx * z / 2));

		currentCameraLongitude += cameraOrbitLongitudeDPS / fps;
	}

}

void ofApp::drawSmoke(Field3D &field) {
	double density, alpha0 = 0.02f;
	for (int i = 1; i < field.XLast() - 1; ++i) {
		for (int j = 1; j < field.YLast() - 1; ++j) {
			for (int k = 1; k < field.ZLast() - 1; ++k) {
				density = ( field(i,j,k) + field(i+1,j,k) + field(i,j+1,k) +
				            field(i,j,k+1) + field(i+1,j+1,k) + field(i+1,j,k+1) +
							field(i,j+1,k+1) + field(i+1,j+1,k+1) ) / 8.0;
				if (density != 0) {
					density = density >= 1 ? 1.0f : density;
					shader.setUniform1f("alpha", alpha0 + (1 - alpha0) * density);
					ofDrawBox(i * dx, j * dx, k * dx, dx); // draw smoke particle
				} 
			}
		}
	}
}

string ofApp::createFrameDirectory() {
	string path = "screenshots" + to_string(ofGetHours()) + to_string(ofGetMinutes()) + to_string(ofGetSeconds());
	if(!ofDirectory::doesDirectoryExist(path)) {
		ofDirectory::createDirectory(path);
		return path;
	} else {
		cerr << "Directory " + path + " already exists" << endl;
		return "";
	}
}