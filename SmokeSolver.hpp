#pragma once

#include "BaseSmokeSolver.hpp"

class SmokeSolver : public BaseSmokeSolver {
public:
	SmokeSolver(int X, int Y, int Z);

	void update();

private:
	Field3D u, v, w, p;
	Field3D utmp, vtmp, wtmp;

	void generateSmoke();

	void velocityStep();
	void densityStep();

	void addSource();
	void addBuoyancy();

	void diffuse(int b);
};
