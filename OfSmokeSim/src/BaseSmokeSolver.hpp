#pragma once

#include <stdexcept>
#include "Field3D.hpp"

// Base class for any smoke simulation-oriented class.
// Minimal functionality:
//   - receives and handles simulation constants,
//   - provides update() function,
//   - allows access to density field.
class BaseSmokeSolver{
public:
	BaseSmokeSolver(int X, int Y, int Z)
		: d(X,Y,Z), dtmp(X,Y,Z) {}

	// Setters.

	void setDt(num_t dt){
		if(dt <= 0.0)  throw std::invalid_argument("Delta-t must be positive.");
		this->dt = dt;
	}

	void setDx(num_t dx){
		if(dx <= 0.0)  throw std::invalid_argument("Value of dx must be positive.");
		this->dx = dx;
	}

	void setDensityDecay(num_t density_decay){
		if(density_decay < 0.0)  throw std::invalid_argument("Density decay must not be negative.");
		this->density_decay = density_decay;
	}

	// Getters.

	num_t getDt() const { return dt; }
	num_t getDx() const { return dx; }
	num_t getDensityDecay() const { return density_decay; }

	// Perform single simulation step.
	virtual void update() = 0;

	const Field3D& getDensityField() const { return d; }

protected:
	// Density of smoke.
	Field3D d, dtmp;

	// Rate at which smoke density is reduced.
	num_t density_decay;

	// Delta-t.
	num_t dt;

	// Delta-x = Delta-y = Delta-z
	num_t dx;
};
