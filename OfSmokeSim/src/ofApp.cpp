#include "ofApp.h"

//--------------------------------------------------------------
int x = 30, y = 30, z = 30;
int fps = 4;
double dx = 0.5;
SmokeSolver smokeSolver(x, y, z);
BoundaryBox boundaryBox(x * dx, y * dx, z * dx);

void ofApp::setup() {
	ofSetFrameRate(fps);
	ofEnableDepthTest();
	ofEnableLighting();
	ofBackground(0, 0, 0);
	light.setup();

	smokeSolver.setBuoyancy(0.3);
	smokeSolver.setDt(0.01);
	smokeSolver.setDx(dx);
	smokeSolver.setKinematicViscosity(0.001);

	cam.disableMouseInput();
	shader.load("smokeShader");
	cout << "START" << endl;
}

//--------------------------------------------------------------
void ofApp::update(){
	smokeSolver.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	auto field = smokeSolver.getDensityField();
	
	cam.begin();
	light.enable();

	cameraControl();
	light.setPosition(cam.getPosition());

	boundaryBox.draw();
	
	glDisable(GL_DEPTH_TEST); 
	glCullFace(GL_FRONT);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	shader.begin();
	drawSmoke(field);
	shader.end();

	light.disable();
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

void ofApp::cameraControl() {
	cam.setTarget(ofPoint(dx * x / 2, dx * y / 2, dx * z / 2));

	if(!pauseRotation) {
		cam.orbitDeg(cameraOrbitLongitude * ofGetElapsedTimef(), 
					cameraOrbitLatitude, 
					30, 
					ofPoint(dx * x / 2, dx * y / 2, dx * z / 2));
					//cout << cameraOrbitLongitude * ofGetElapsedTimef() << " " << cameraOrbitLatitude << endl;
	}
}

void ofApp::drawSmoke(Field3D &field) {
	double currentHighestDensity = highestDensity;
	double density;
	for (int i = 1; i < field.XLast() - 1; ++i) {
		for (int j = 1; j < field.YLast() - 1; ++j) {
			for (int k = 1; k < field.ZLast() - 1; ++k) {
				density = field(i, j, k);
				if (density != 0) {
					highestDensity = highestDensity < density ? density : highestDensity;
					shader.setUniform1f("density", density);
					shader.setUniform1f("alpha", density);
					ofDrawBox(i * dx, j * dx, k * dx, dx); // draw smoke particle
				}
			}
		}
	}
}