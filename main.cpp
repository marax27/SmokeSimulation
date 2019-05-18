#include <iostream>
#include <cmath>

#include "SmokeSolver.hpp"

int main(){
	SmokeSolver ss(30, 30, 30);
	ss.setBuoyancy(0.3);
	ss.setDt(0.01);
	ss.setDx(0.15);
	ss.setKinematicViscosity(0.001);

	for(int iteration = 0; iteration < 50; ++iteration){
		std::cout << "Step #" << iteration << std::endl;

		ss.update();

		if(iteration % 10 == 1){
			std::cout << '.';
			std::cout.flush();
		}
	}
	std::cout << '\n';

	auto rho = ss.getDensityField();
	printf("Dimensions: %dx%dx%d\n", rho.XLast()-1, rho.YLast()-1, rho.ZLast()-1);

	for(int j = 1; j < rho.YLast(); ++j){
		for(int k = 1; k < rho.ZLast(); ++k){
			for(int i = 1; i < rho.XLast(); ++i){
				auto r = rho(i,j,k);
				std::cout << r << ' ';
			}
			std::cout << '\n';
		}
		std::cout << "\n----------\n";
	}
}
