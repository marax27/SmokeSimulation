#include "ofApp.h"

//--------------------------------------------------------------
int x = 30, y = 30, z = 30;
SmokeSolver ss(x, y, z);

void ofApp::setup() {
	ofSetFrameRate(4);
	ofEnableDepthTest();
	ofBackground(0, 0, 0);

	dx = 0.5;
	ss.setBuoyancy(0.3);
	ss.setDt(0.01);
	ss.setDx(dx);
	ss.setKinematicViscosity(0.001);

	cam.lookAt(ofPoint(dx * x / 2, dx * y / 2, dx * z / 2));
	cam.setDistance(30);

	cout << "START" << endl;
}

//--------------------------------------------------------------
void ofApp::update(){
	ss.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	auto field = ss.getDensityField();
	double currentHighestDensity = highestDensity;
	double density, alpha;
	cam.begin();

	drawBoundaryBox(x * dx, y * dx, z * dx, dx);

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

void ofApp::drawBoundaryBox(double x, double y, double z, double dx) {
	ofPoint point1(0, 0, 0);
	ofPoint point2(x, 0, 0);
	ofPoint point3(0, 0, z);
	ofPoint point4(x, 0, z);
	ofPoint point5(0, y, 0);
	ofPoint point6(x, y, 0);
	ofPoint point7(0, y, z);
	ofPoint point8(x, y, z);

	ofColor color = ofColor(255, 255, 0, 200);
	ofEnableAlphaBlending();
	ofSetColor(ofColor::red);
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, y * 2, 0));
	ofSetColor(ofColor::blue);
	ofDrawLine(ofPoint(x, 0, z), ofPoint(x, y * 2, z));
	ofSetColor(color);
	ofDrawLine(point1, point2);
	ofDrawLine(point1, point3);
	ofDrawLine(point2, point4);
	ofDrawLine(point3, point4);

	ofDrawLine(point5, point6);
	ofDrawLine(point5, point7);
	ofDrawLine(point6, point8);
	ofDrawLine(point7, point8);

	ofDrawLine(point1, point5);
	ofDrawLine(point2, point6);
	ofDrawLine(point3, point7);
	ofDrawLine(point4, point8);

	ofDisableAlphaBlending();
}

double ofApp::setParticleAlpha(double density, double highestDensity) {
	return (density * 255) / highestDensity;
}