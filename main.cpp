#include <iostream>

#include "SmokeSolver.hpp"

int main(){
	SmokeSolver ss(50, 25, 30);

	ss.update();

	auto rho = ss.getDensityField();
	std::cout << rho.LastXIndex() << '\n';
}
