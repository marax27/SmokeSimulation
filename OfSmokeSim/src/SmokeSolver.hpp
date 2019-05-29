#pragma once

#include "BaseSmokeSolver.hpp"
#include "Vector3.hpp"

class SmokeSolver : public BaseSmokeSolver {
public:
	SmokeSolver(int X, int Y, int Z);

	void update();

	num_t getThresholdDt(bool fedkiw_relaxation = false) const;

	// Setters.

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

	// void setSmokeDiffusionCoefficient(num_t coef){
	// 	if(coef < 0.0)  throw std::invalid_argument("Smoke diffusion coefficient must not be negative.");
	// 	this->smoke_diffusion_coefficient = coef;
	// }

	void setVorticityConfinementCoefficient(num_t coef){
		if(coef < 0.0)  throw std::invalid_argument("Vorticity confinement coefficient must be positive.");
		this->vort_conf_coef = coef;
	}

	void setWindVelocity(num3d vel){
		this->wind_velocity = vel;
	}

	void setWindCoefficient(num_t coef){
		if(coef < 0)  throw std::invalid_argument("Wind coefficient must not be negative.");
		this->k_wind = coef;
	}

	// Getters.

	num_t getKinematicViscosity() const { return kinematic_viscosity; }
	num_t getFluidDensity() const { return fluid_density; }
	num_t getFallCoefficient() const { return k_fall; }
	num_t getRiseCoefficient() const { return k_rise; }
	// num_t getSmokeDiffusionCoefficient() const { return smoke_diffusion_coefficient; }
	num_t getVorticityConfinementCoefficient() const { return vort_conf_coef; }
	num3d getWindVelocity() const { return wind_velocity; }
	num_t getWindCoefficient() const { return k_wind; }

	const Field3D& getTemperatureField() const { return T; }

protected:
	Field3D u, v, w, p;
	Field3D utmp, vtmp, wtmp;
	Field3D diverg;
	Field3D T, Ttmp;

	num_t max_squared_speed;

	void generateSmoke();

	void velocityStep();
	void densityStep();

	void addForces();

	void addBuoyancy();
	void addWind();
	void addVorticityConfinement();

	void project();

	enum Direction{ NONE, X, Y, Z };

	// Use Semi-Lagrangian scheme to advect field.
	void advect(Direction dir, Field3D &field, Field3D &field_tmp, Field3D &velX, Field3D &velY, Field3D &velZ);

	void diffuse(Direction dir, Field3D &field, Field3D &field_tmp, num_t diffusion_coefficient);

	void enforceBoundary(Direction dir, Field3D field);

	// Coordinates.

	num3d ijk2RealPos(idx3d pos) const;


	// Fields.

	// Vorticity confinement coefficient.
	num_t vort_conf_coef;

	// Fluid constants.
	num_t kinematic_viscosity, fluid_density;

	// Smoke density parameters.
	// num_t smoke_diffusion_coefficient;

	// Buyoancy coefficients.
	num_t k_fall, k_rise;

	num3d wind_velocity;

	// Wind coefficient - describes how quickly fluid velocity approaches wind velocity.
	num_t k_wind;
};
