#include <iostream>
#include <cmath>

#include "SmokeSolver.hpp"

int main(){
	// Initialize solver.
	SmokeSolver ss(30, 30, 30);
	ss.setBuoyancy(0.3);
	ss.setDt(0.01);
	ss.setDx(0.15);
	ss.setKinematicViscosity(0.001);

	const int MAX_STEPS = 20;

	// Simulate.
	for(int step = 0; step < MAX_STEPS; ++step){
		ss.update();

		std::cout << "\r" << (step+1) * 100 / MAX_STEPS << "%";
		std::cout.flush();
	}
	std::cout << "\n";

	auto density_field = ss.getDensityField();
	// Do something with density field.
}
