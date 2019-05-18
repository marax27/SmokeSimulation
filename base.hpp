#pragma once

typedef double num_t;

constexpr num_t squared(num_t x){ return x*x; }

constexpr num_t clamp(num_t min, num_t max, num_t value){
	return (value > max ? max : (value < min ? min : value));
}
