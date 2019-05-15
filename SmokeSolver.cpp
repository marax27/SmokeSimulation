#include "SmokeSolver.hpp"

SmokeSolver::SmokeSolver(int X, int Y, int Z)
: BaseSmokeSolver(X,Y,Z), u(X, Y+1, Z+1), v(X+1, Y, Z+1), p(X+1, Y+1, Z+1) {}

void SmokeSolver::update(){
	generateSmoke();

	velocityStep();
	densityStep();
}

void SmokeSolver::generateSmoke(){
	const int N = p.LastYIndex();
	const int centerY = p.LastYIndex() / 4;
	const int centerZ = p.LastZIndex() / 2;
	float dens = (rand()%1000)/1000.0f;

	for (int i = 1; i < smoke_density.LastYIndex(); ++i) {
		for (int j = 1; j < smoke_density.LastZIndex(); ++j) {
			if (squared(i-centerY) + squared(j-centerZ) < squared(N/10)) {
				smoke_density(5,i,j) = dens;
				u(5,i,j) = 2.0f;
			}
		}
	}
}

void SmokeSolver::velocityStep(){
	addSource();
	addSource();
	addSource();

	addBuoyancy();

	utmp.copyFrom(u);  vtmp.copyFrom(v);  wtmp.copyFrom(w);
	diffuse(1);  diffuse(2);  diffuse(3);
}

void SmokeSolver::densityStep(){

}


void SmokeSolver::addSource(){

}

void SmokeSolver::addBuoyancy(){
	for(int i = 0; i < v.XSize(); ++i){
		for(int j = 0; j < v.YSize(); ++j){
			for(int k = 0; k < v.ZSize(); ++k){
				v(i,j,k) += smoke_density(i,j,k) * buoyancy * dt;
			}
		}
	}
}

void SmokeSolver::diffuse(int b){
	num_t a = dt * kinematic_viscosity * 
}
