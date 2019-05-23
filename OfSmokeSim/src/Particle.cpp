#include "Particle.h"

Particle::Particle(){
}


Particle::~Particle(){
}

void Particle::draw(double x, double y, double z, double dx, double alpha) {
	ofColor color = ofColor(211, 211, 211);
	//ofEnableAlphaBlending();
	ofSetColor(color);
	ofDrawBox(x * dx, y * dx, z * dx, dx);
	//ofDisableAlphaBlending();
}