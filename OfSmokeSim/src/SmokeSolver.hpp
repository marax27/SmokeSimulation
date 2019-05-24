#pragma once

#include "BaseSmokeSolver.hpp"
#include "Vector3.hpp"

class SmokeSolver : public BaseSmokeSolver {
public:
	SmokeSolver(int X, int Y, int Z);

	void update();

private:
	Field3D u, v, w, p;
	Field3D utmp, vtmp, wtmp;
	Field3D diverg;

	void generateSmoke();

	void velocityStep();
	void densityStep();

	void addBuoyancy();
	void project();

	enum Direction{ NONE, X, Y, Z };

	// Use Semi-Lagrangian scheme to advect field.
	void advect(Direction dir, Field3D &field, Field3D &field_tmp, Field3D &velX, Field3D &velY, Field3D &velZ);

	void diffuse(Direction dir, Field3D &field, Field3D &field_tmp, num_t diffusion_coefficient);

	void enforceBoundary(Direction dir, Field3D field);

	// Coordinates.

	num3d ijk2RealPos(idx3d pos) const;
};
