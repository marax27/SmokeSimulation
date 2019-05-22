#pragma once

#include "ofMain.h"

class Particle {
public:
	Particle();
	~Particle();

	void draw(double x, double y, double z, double dx, double alpha);
};

