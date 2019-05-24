#include <iostream>

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
	diverg(X+2,Y+2,Z+2)
	// Staggered
	/* : BaseSmokeSolver(X+1,Y+1,Z+1),
	   u(X, Y+1, Z+1), v(X+1, Y, Z+1), w(X+1, Y+1, Z), p(X+1, Y+1, Z+1),
	   utmp(X, Y+1, Z+1), vtmp(X+1, Y, Z+1), wtmp(X+1, Y+1, Z) */
 	{}

void SmokeSolver::update(){
	generateSmoke();

	velocityStep();
	densityStep();
}

void SmokeSolver::generateSmoke(){
	const int N = p.YLast();
	const int centerY = p.YLast() / 4;
	const int centerZ = p.ZLast() / 2;
	float source_density = (rand()%1000)/1000.0f;

	for (int i = 1; i < d.YLast(); ++i) {
		for (int j = 1; j < d.ZLast(); ++j) {
			if (squared(i-centerY) + squared(j-centerZ) < squared(N/10)) {
				d(5,i,j) = source_density;
				u(5,i,j) = 2.0f;
			}
		}
	}
}

void SmokeSolver::velocityStep(){
	utmp.copyFrom(u);  vtmp.copyFrom(v);  wtmp.copyFrom(w);

	// Advect.
	advect(Direction::X, u, utmp, utmp, vtmp, wtmp);
	advect(Direction::Y, v, vtmp, utmp, vtmp, wtmp);
	advect(Direction::Z, w, wtmp, utmp, vtmp, wtmp);

	// Add forces.
	addBuoyancy();

	// Diffuse.
	diffuse(Direction::X, u, utmp, kinematic_viscosity);
	diffuse(Direction::Y, v, vtmp, kinematic_viscosity);
	diffuse(Direction::Z, w, wtmp, kinematic_viscosity);

	project();
}

void SmokeSolver::densityStep(){
	dtmp.copyFrom(d);

	// Advect.
	advect(Direction::NONE, d, dtmp, u, v, w);

	// Diffuse.
	#warning Magic value.
	diffuse(Direction::NONE, d, dtmp, 0.1);

	// Decrease density.
	FOR_EACH_COMPUTABLE_CELL(d){
		d(i,j,k) -= 0.0005;
		if(d(i,j,k) < 0)
			d(i,j,k) = 0;
	}END_FOR
}

#warning Custom implementation.
void SmokeSolver::addBuoyancy(){
	for(int i = 1; i < v.XLast(); ++i){
		for(int j = 1; j < v.YLast(); ++j){
			for(int k = 1; k < v.ZLast(); ++k){
				auto dens = d(i,j,k);
				v(i,j,k) += dt * (k_rise * (1.0/dens - 1.0/fluid_density) - k_fall * dens);
			}
		}
	}
}

void SmokeSolver::project(){

	// Compute divergence and clear pressure field.
	FOR_EACH_COMPUTABLE_CELL(diverg){
		diverg(i,j,k) = dx/3.0 * (
			u(i+1,j,k) - u(i-1,j,k) +
			v(i,j+1,k) - v(i,j-1,k) +
			w(i,j,k+1) - w(i,j,k-1)
		);
		p(i,j,k) = 0;  //as long as grid is collocated, no segfaults on the horizon
	}END_FOR
	enforceBoundary(Direction::NONE, diverg);
	enforceBoundary(Direction::NONE, p);

	for(int step = 0; step < 20; ++step){
		FOR_EACH_COMPUTABLE_CELL(p){
			p(i,j,k) = (-diverg(i,j,k) +
				p(i-1,j,k) + p(i+1,j,k) +
				p(i,j-1,k) + p(i,j+1,k) +
				p(i,j,k-1) + p(i,j,k+1)
			) / 6.0;
		}END_FOR
		enforceBoundary(Direction::NONE, p);
	}

	// As long as grid is collocated, and all fields have same dimensions, all should be fine.
	FOR_EACH_COMPUTABLE_CELL(u){
		u(i,j,k) -= dt * (p(i+1,j,k) - p(i-1,j,k)) /3/dx;
		v(i,j,k) -= dt * (p(i,j+1,k) - p(i,j-1,k)) /3/dx;
		w(i,j,k) -= dt * (p(i,j,k+1) - p(i,j,k-1)) /3/dx;
	}END_FOR

	enforceBoundary(Direction::X, u);
	enforceBoundary(Direction::Y, v);
	enforceBoundary(Direction::Z, w);
}

void SmokeSolver::advect(Direction dir, Field3D &field, Field3D &field_tmp, Field3D &velX, Field3D &velY, Field3D &velZ){
	// Based on: https://en.wikipedia.org/wiki/Trilinear_interpolation

	num3d current_pos, previous_pos;
	idx3d idx0, idx1;
	num3d xyz0;  //real position of a point with coordinates idx0
	num3d dd;    //relative distances; (x_d, y_d, z_d) on Wikipedia
	num3d dd_1;  //holds values 1-dd.x, 1-dd.y, 1-dd.z
	num_t c0, c1, c00, c01, c10, c11;

	// Number of computable cells.
	const int CCx = field.XSize() - 2, CCy = field.YSize() - 2, CCz = field.ZSize() - 2;

	FOR_EACH_COMPUTABLE_CELL(field){
		// Trace backward position of a particle.
		current_pos = ijk2RealPos({i,j,k});
		previous_pos = current_pos - num3d(velX(i,j,k), velY(i,j,k), velZ(i,j,k)) * dt;

		previous_pos = num3d(clamp(.5, .5+CCx*dx, previous_pos.x),
		                     clamp(.5, .5+CCy*dx, previous_pos.y),
		                     clamp(.5, .5+CCz*dx, previous_pos.z));

		idx0 = {int(previous_pos.x/dx), int(previous_pos.y/dx), int(previous_pos.z/dx)};
		idx1 = {idx0.i + 1, idx0.j + 1, idx0.k + 1};

		xyz0 = ijk2RealPos(idx0);

		dd = (previous_pos - xyz0) / dx;
		dd_1 = num3d(1,1,1) - dd;

		c00 = field_tmp(idx0.i, idx0.j, idx0.k)*dd_1.x + field_tmp(idx1.i, idx0.j, idx0.k)*dd.x;
		c01 = field_tmp(idx0.i, idx0.j, idx1.k)*dd_1.x + field_tmp(idx1.i, idx0.j, idx1.k)*dd.x;
		c10 = field_tmp(idx0.i, idx1.j, idx0.k)*dd_1.x + field_tmp(idx1.i, idx1.j, idx0.k)*dd.x;
		c11 = field_tmp(idx0.i, idx1.j, idx1.k)*dd_1.x + field_tmp(idx1.i, idx1.j, idx1.k)*dd.x;

		c0 = c00*dd_1.z + c01*dd.z;
		c1 = c10*dd_1.z + c11*dd.z;

		field(i,j,k) = c0*dd_1.y + c1*dd.y;
	}END_FOR

#warning Density?
	enforceBoundary(dir, field);
}

void SmokeSolver::diffuse(SmokeSolver::Direction dir, Field3D &field, Field3D &field_tmp, num_t diffusion_coefficient){
#warning Je suis desole, unimplemented.
	/*const num_t coef = diffusion_coefficient * dt / squared(dx);

	for(int step = 0; step < 20; ++step){
		FOR_EACH_COMPUTABLE_CELL(field){
			field
		}END_FOR;
	}*/

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

