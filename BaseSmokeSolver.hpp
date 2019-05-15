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
		: smoke_density(X+1,Y+1,Z+1) {}

	void setDt(num_t dt){
		if(dt <= 0.0)
			throw std::invalid_argument("Delta-t must be positive.");
		this->dt = dt;
	}

	void setH(num_t h){
		if(h <= 0.0)
			throw std::invalid_argument("Value of h must be positive.");
		this->h = h;
	}

	void setKinematicViscosity(num_t nu){
		if(nu < 0.0)
			throw std::invalid_argument("Kinematic viscosity must be positive.");
		this->kinematic_viscosity = nu;
	}

	void setBuoyancy(num_t buoyancy){
		this->buoyancy = buoyancy;
	}

	// Perform single simulation step.
	virtual void update() = 0;

	const Field3D& getDensityField() const { return smoke_density; }

protected:
	Field3D smoke_density;

	// Fluid constants.
	num_t kinematic_viscosity, density, buoyancy;

	num_t dt, h;
};
