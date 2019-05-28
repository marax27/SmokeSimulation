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

	BaseSmokeSolver& setDt(num_t dt){
		if(dt <= 0.0)  throw std::invalid_argument("Delta-t must be positive.");
		this->dt = dt;
		return *this;
	}

	BaseSmokeSolver& setDx(num_t dx){
		if(dx <= 0.0)  throw std::invalid_argument("Value of dx must be positive.");
		this->dx = dx;
		return *this;
	}

	BaseSmokeSolver& setKinematicViscosity(num_t nu){
		if(nu < 0.0)  throw std::invalid_argument("Kinematic viscosity must not be negative.");
		this->kinematic_viscosity = nu;
		return *this;
	}

	BaseSmokeSolver& setFluidDensity(num_t dens){
		if(dens <= 0.0)  throw std::invalid_argument("Fluid density must be positive.");
		this->fluid_density = dens;
		return *this;
	}

	BaseSmokeSolver& setFallCoefficient(num_t k_fall){
		if(k_fall < 0.0)  throw std::invalid_argument("Fall coefficient must not be negative.");
		this->k_fall = k_fall;
		return *this;
	}

	BaseSmokeSolver& setRiseCoefficient(num_t k_rise){
		if(k_rise < 0.0)  throw std::invalid_argument("Rise coefficient must not be negative.");
		this->k_rise = k_rise;
		return *this;
	}

	BaseSmokeSolver& setSmokeDiffusionCoefficient(num_t coef){
		if(coef < 0.0)  throw std::invalid_argument("Smoke diffusion coefficient must not be negative.");
		this->smoke_diffusion_coefficient = coef;
		return *this;
	}

	BaseSmokeSolver& setDensityThreshold(num_t density_threshold){
		if(density_threshold < 0.0)  throw std::invalid_argument("Density threshold must not be negative.");
		this->density_threshold = density_threshold;
		return *this;
	}

	BaseSmokeSolver& setVorticityConfinementCoefficient(num_t coef){
		if(coef <= 0.0)  throw std::invalid_argument("Vorticity confinement coefficient must be positive.");
		this->vort_conf_coef = coef;
		return *this;
	}

	// Getters.

	num_t getDt() const { return dt; }
	num_t getDx() const { return dx; }
	num_t getKinematicViscosity() const { return kinematic_viscosity; }
	num_t getFluidDensity() const { return fluid_density; }
	num_t getFallCoefficient() const { return k_fall; }
	num_t getRiseCoefficient() const { return k_rise; }
	num_t getSmokeDiffusionCoefficient() const { return smoke_diffusion_coefficient; }
	num_t getDensityThreshold() const { return density_threshold; }
	num_t getVorticityConfinementCoefficient() const { return vort_conf_coef; }

	// Perform single simulation step.
	virtual void update() = 0;

	const Field3D& getDensityField() const { return d; }

protected:
	// Density of smoke.
	Field3D d, dtmp;

	// Fluid constants.
	num_t kinematic_viscosity, fluid_density;

	// Smoke density parameters.
	num_t smoke_diffusion_coefficient, density_threshold;

	// Forces.
	num_t k_fall, k_rise;

	// Delta-t.
	num_t dt;

	// Delta-x = Delta-y = Delta-z
	num_t dx;

	num_t vort_conf_coef;
};
