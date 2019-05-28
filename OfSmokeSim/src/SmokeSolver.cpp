#include <iostream>
#include <cmath>

#include "SmokeSolver.hpp"

#define FOR_EACH_COMPUTABLE_CELL(field)             \
	for(int i = 1; i < field.XLast(); ++i){         \
		for(int j = 1; j < field.YLast(); ++j){     \
			for(int k = 1; k < field.ZLast(); ++k)

#define END_FOR }}



SmokeSolver::SmokeSolver(int X, int Y, int Z)
	// Collocated
	: BaseSmokeSolver(X+2,Y+2,Z+2),
	u(X+2,Y+2,Z+2), v(X+2,Y+2,Z+2), w(X+2,Y+2,Z+2), p(X+2,Y+2,Z+2),
	utmp(X+2,Y+2,Z+2), vtmp(X+2,Y+2,Z+2), wtmp(X+2,Y+2,Z+2),
	diverg(X+2,Y+2,Z+2),
	T(X+2,Y+2,Z+2), Ttmp(X+2,Y+2,Z+2)
	// Staggered
	/* : BaseSmokeSolver(X+1,Y+1,Z+1),
	   u(X, Y+1, Z+1), v(X+1, Y, Z+1), w(X+1, Y+1, Z), p(X+1, Y+1, Z+1),
	   utmp(X, Y+1, Z+1), vtmp(X+1, Y, Z+1), wtmp(X+1, Y+1, Z) */
 	{}

//************************************************************

void SmokeSolver::update(){
	generateSmoke();

	velocityStep();
	densityStep();
}

num_t SmokeSolver::getThresholdDt(bool fedkiw_relaxation) const {
	// Use (relaxed or not) CFL condition to get maximum stable delta-t.
	// Do not use at the very beginning of the simulation, when
	// max_squared_speed is uninitialized.
	return (fedkiw_relaxation ? 5 : 1) * dx / sqrt(max_squared_speed);
}

//************************************************************

void SmokeSolver::generateSmoke(){
	const int N = p.YLast();
	const int centerY = p.YLast() / 4;
	const int centerZ = p.ZLast() / 2;
	float source_density = (rand()%1000)/1000.0f;

	for (int j = 1; j < d.YLast(); ++j) {
		for (int k = 1; k < d.ZLast(); ++k) {
			if (squared(j-centerY) + squared(k-centerZ) < squared(N/10)) {
				d(5,j,k) = source_density;
				u(5,j,k) = 5.0;
				T(5,j,k) = 100;
			}
		}
	}
}

void SmokeSolver::velocityStep(){
	addBuoyancy();
#warning Wind disabled.
	// addWind();

	utmp.swapWith(u);  vtmp.swapWith(v);  wtmp.swapWith(w);
	diffuse(Direction::X, u, utmp, kinematic_viscosity);
	diffuse(Direction::Y, v, vtmp, kinematic_viscosity);
	diffuse(Direction::Z, w, wtmp, kinematic_viscosity);
	project();

	utmp.swapWith(u);  vtmp.swapWith(v);  wtmp.swapWith(w);
	advect(Direction::X, u, utmp, utmp, vtmp, wtmp);
	advect(Direction::Y, v, vtmp, utmp, vtmp, wtmp);
	advect(Direction::Z, w, wtmp, utmp, vtmp, wtmp);
	project();
}

void SmokeSolver::densityStep(){
	dtmp.swapWith(d);
	diffuse(Direction::NONE, d, dtmp, smoke_diffusion_coefficient);

	dtmp.swapWith(d);
	advect(Direction::NONE, d, dtmp, u, v, w);


	Ttmp.swapWith(T);
	diffuse(Direction::NONE, T, Ttmp, smoke_diffusion_coefficient);

	Ttmp.swapWith(T);
	advect(Direction::NONE, T, Ttmp, u, v, w);

	FOR_EACH_COMPUTABLE_CELL(d){
		d(i,j,k) -= .0001;
		if(d(i,j,k) < 0)
			d(i,j,k) = 0;
	}END_FOR
}

void SmokeSolver::addBuoyancy(){
	FOR_EACH_COMPUTABLE_CELL(v){
		auto dens = d(i,j,k);
		if(dens > density_threshold)
			v(i,j,k) += dt * (k_rise * T(i,j,k) - k_fall * dens);
	}END_FOR
}

void SmokeSolver::addWind(){
	const num_t coef = 0.75 * dt;
	FOR_EACH_COMPUTABLE_CELL(u){  //assumption: dim u = dim v = dim w
		u(i,j,k) *= 1 - coef;
		v(i,j,k) *= 1 - coef;
		
		w(i,j,k) *= 1 - coef;
		w(i,j,k) += coef * 10;
	}END_FOR
}

void SmokeSolver::project(){

	FOR_EACH_COMPUTABLE_CELL(diverg){
		diverg(i,j,k) = (
			u(i+1,j,k) - u(i-1,j,k) +
			v(i,j+1,k) - v(i,j-1,k) +
			w(i,j,k+1) - w(i,j,k-1)
		) * .5/dx;
		p(i,j,k) = 0;
	}END_FOR
	enforceBoundary(Direction::NONE, diverg);
	enforceBoundary(Direction::NONE, p);

	const num_t div_coef = -fluid_density * squared(dx) / dt;
	for(int step = 0; step < 20; ++step){
		FOR_EACH_COMPUTABLE_CELL(p){
			p(i,j,k) = (div_coef * diverg(i,j,k) +
				p(i-1,j,k) + p(i+1,j,k) +
				p(i,j-1,k) + p(i,j+1,k) +
				p(i,j,k-1) + p(i,j,k+1)
			) / 6;
		}END_FOR
		enforceBoundary(Direction::NONE, p);
	}

	// Tracking maximum speed in velocity field.
	max_squared_speed = 0;
	num_t squared_speed;

	const num_t vel_coef = .5/dx * dt/fluid_density;
	FOR_EACH_COMPUTABLE_CELL(u){
		u(i,j,k) -= (p(i+1,j,k) - p(i-1,j,k)) * vel_coef;
		v(i,j,k) -= (p(i,j+1,k) - p(i,j-1,k)) * vel_coef;
		w(i,j,k) -= (p(i,j,k+1) - p(i,j,k-1)) * vel_coef;

		// Update maximum speed.
		squared_speed = squared(u(i,j,k)) + squared(v(i,j,k)) + squared(w(i,j,k));
		if(squared_speed > max_squared_speed)
			max_squared_speed = squared_speed;
	}END_FOR
	enforceBoundary(Direction::X, u);
	enforceBoundary(Direction::Y, v);
	enforceBoundary(Direction::Z, w);  //it wasn't here in original source code
}

void SmokeSolver::advect(Direction dir, Field3D &field, Field3D &field_tmp, Field3D &velX, Field3D &velY, Field3D &velZ){
	idx3d idx0, idx1;
	num3d current_pos, previous_pos;
	num3d s0, s1;
	num_t coef0, coef1;

	const int CCx = field.XSize()-2, CCy = field.YSize()-2, CCz = field.ZSize()-2;

	FOR_EACH_COMPUTABLE_CELL(field){
		current_pos = ijk2RealPos({i,j,k});
		previous_pos = current_pos - num3d(velX(i,j,k), velY(i,j,k), velZ(i,j,k))*dt;

		if(previous_pos.x < .5)          previous_pos.x = .5;
		if(previous_pos.x > CCx*dx + .5)  previous_pos.x = CCx*dx + .5;
		if(previous_pos.y < .5)          previous_pos.y = .5;
		if(previous_pos.y > CCy*dx + .5)  previous_pos.y = CCy*dx + .5;
		if(previous_pos.z < .5)          previous_pos.z = .5;
		if(previous_pos.z > CCz*dx + .5)  previous_pos.z = CCz*dx + .5;

		idx0.i = clamp(0, CCx, int(previous_pos.x / dx));  idx1.i = idx0.i+1;
		idx0.j = clamp(0, CCy, int(previous_pos.y / dx));  idx1.j = idx0.j+1;
		idx0.k = clamp(0, CCz, int(previous_pos.z / dx));  idx1.k = idx0.k+1;

		s1 = (previous_pos - num3d(idx0.i, idx0.j, idx0.k) * dx) / dx;
		s0 = num3d(1,1,1) - s1;

		coef0 = s0.x * ( s0.y*field_tmp(idx0.i,idx0.j,idx0.k) + s1.y*field_tmp(idx0.i,idx1.j,idx0.k) ) +
		        s1.x * ( s0.y*field_tmp(idx1.i,idx0.j,idx0.k) + s1.y*field_tmp(idx1.i,idx1.j,idx0.k) );
		coef1 = s0.x * ( s0.y*field_tmp(idx0.i,idx0.j,idx1.k) + s1.y*field_tmp(idx0.i,idx1.j,idx1.k) ) +
		        s1.x * ( s0.y*field_tmp(idx1.i,idx0.j,idx1.k) + s1.y*field_tmp(idx1.i,idx1.j,idx1.k) );
		field(i,j,k) = s0.z*coef0 + s1.z*coef1;
	}END_FOR
	enforceBoundary(dir, d);
}

void SmokeSolver::diffuse(SmokeSolver::Direction dir, Field3D &field, Field3D &field_tmp, num_t diffusion_coefficient){
	const num_t coef = diffusion_coefficient * dt / squared(dx);

	for(int step = 0; step < 20; ++step){
		FOR_EACH_COMPUTABLE_CELL(field){
			field(i,j,k) = (field_tmp(i,j,k) + coef*(
				field(i-1,j,k) + field(i+1,j,k) +
				field(i,j-1,k) + field(i,j+1,k) +
				field(i,j,k-1) + field(i,j,k+1)
			)) / (1 + 6*coef);
		}END_FOR;
	}

	enforceBoundary(dir, field);
}

void SmokeSolver::enforceBoundary(SmokeSolver::Direction dir, Field3D field){
	for(int j = 1; j < field.YLast(); ++j){
		for(int k = 1; k < field.ZLast(); ++k){
			field(0,j,k) = (dir == Direction::X) ? -field(1,j,k) : field(1,j,k);
			field(-1,j,k) = (dir == Direction::X) ? -field(-2,j,k) : field(-2,j,k);
		}
	}

	for(int i = 1; i < field.XLast(); ++i){
		for(int k = 1; k < field.ZLast(); ++k){
			field(i,0,k) = (dir == Direction::Y) ? -field(i,1,k) : field(i,1,k);
			field(i,-1,k) = (dir == Direction::Y) ? -field(i,-2,k) : field(i,-2,k);
		}
	}
	
	for(int i = 1; i < field.XLast(); ++i){
		for(int j = 1; j < field.YLast(); ++j){
			field(i,j,0) = (dir == Direction::Z) ? -field(i,j,1) : field(i,j,1);
			field(i,j,-1) = (dir == Direction::Z) ? -field(i,j,-2) : field(i,j,-2);
		}
	}

	field(0,0,0)    = (field(1,0,0)    + field(0,1,0)    + field(0,0,1))    / 3;
	field(0,-1,0)   = (field(1,-1,0)   + field(0,-2,0)   + field(0,-1,1))   / 3;
	field(-1,0,0)   = (field(-2,0,0)   + field(-1,1,0)   + field(-1,0,1))   / 3;
	field(-1,-1,0)  = (field(-2,-1,0)  + field(-1,-2,0)  + field(-1,-1,1))  / 3;
	field(0,0,-1)   = (field(1,0,-1)   + field(0,1,-1)   + field(0,0,-2))   / 3;
	field(0,-1,-1)  = (field(1,-1,-1)  + field(0,-2,-1)  + field(0,-1,-2))  / 3;
	field(-1,0,-1)  = (field(-2,0,-1)  + field(-1,1,-1)  + field(-1,0,-2))  / 3;
	field(-1,-1,-1) = (field(-2,-1,-1) + field(-1,-2,-1) + field(-1,-1,-2)) / 3;
}

num3d SmokeSolver::ijk2RealPos(idx3d pos) const{
	return num3d(pos.i,pos.j,pos.k) * dx;
}

