#pragma once

#include "base.hpp"

struct idx3d{
	int i, j, k;
};

struct num3d{
	num3d() = default;
	num3d(num_t x, num_t y, num_t z) : x(x), y(y), z(z) {}

	num3d operator+(const num3d &right) const {
		return num3d(x+right.x, y+right.y, z+right.z);
	}
	num3d operator-(const num3d &right) const {
		return num3d(x-right.x, y-right.y, z-right.z);
	}
	num3d operator*(const num_t &right) const {
		return num3d(x*right, y*right, z*right);
	}
	num3d operator/(const num_t &right) const {
		return num3d(x/right, y/right, z/right);
	}

	num3d& operator+=(const num3d &right){
		*this = *this + right;
		return *this;
	}
	num3d& operator-=(const num3d &right){
		*this = *this - right;
		return *this;
	}

	bool operator==(const num3d &right) const {
		return x == right.x && y == right.y && z == right.z;
	}
	bool operator!=(const num3d &right) const {
		return !(*this == right);
	}

	num_t x, y, z;
};
