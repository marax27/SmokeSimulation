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

	void setDt(num_t dt){
		if(dt <= 0.0)
			throw std::invalid_argument("Delta-t must be positive.");
		this->dt = dt;
	}

	void setDx(num_t dx){
		if(dx <= 0.0)
			throw std::invalid_argument("Value of dx must be positive.");
		this->dx = dx;
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

	const Field3D& getDensityField() const { return d; }

protected:
	// Density of smoke.
	Field3D d, dtmp;

	// Fluid constants.
	num_t kinematic_viscosity, fluid_density, buoyancy;

	// Delta-t.
	num_t dt;

	// Delta-x = Delta-y = Delta-z
	num_t dx;
};
