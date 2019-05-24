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

	void setKinematicViscosity(num_t nu){
		if(nu < 0.0)  throw std::invalid_argument("Kinematic viscosity must not be negative.");
		this->kinematic_viscosity = nu;
	}

	void setFluidDensity(num_t dens){
		if(dens <= 0.0)  throw std::invalid_argument("Fluid density must be positive.");
		this->fluid_density = dens;
	}

	void setFallCoefficient(num_t k_fall){
		if(k_fall < 0.0)  throw std::invalid_argument("Fall coefficient must not be negative.");
		this->k_fall = k_fall;
	}

	void setRiseCoefficient(num_t k_rise){
		if(k_rise < 0.0)  throw std::invalid_argument("Rise coefficient must not be negative.");
		this->k_rise = k_rise;
	}

	// Getters.

	num_t getDt() const { return dt; }
	num_t getDx() const { return dx; }
	num_t getKinematicViscosity() const { return kinematic_viscosity; }
	num_t getFluidDensity() const { return fluid_density; }
	num_t getFallCoefficient() const { return k_fall; }
	num_t getRiseCoefficient() const { return k_rise; }

	// Perform single simulation step.
	virtual void update() = 0;

	const Field3D& getDensityField() const { return d; }

protected:
	// Density of smoke.
	Field3D d, dtmp;

	// Fluid constants.
	num_t kinematic_viscosity, fluid_density;

	// Forces.
	num_t k_fall, k_rise;

	// Delta-t.
	num_t dt;

	// Delta-x = Delta-y = Delta-z
	num_t dx;
};
