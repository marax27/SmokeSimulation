#pragma once

#include "base.hpp"

// 3D grid with values assigned to each grid cell.
class Field3D{
public:
	// Type to be used when dealing with element indexing or field dimensions.
	typedef int idx_t;

	Field3D() = default;

	Field3D(idx_t x_size, idx_t y_size, idx_t z_size)
		: x_size(x_size), y_size(y_size), z_size(z_size) {
		initialize();
	}

	Field3D(const Field3D &model)
		: x_size(model.x_size), y_size(model.y_size), z_size(model.z_size) {
		initialize();
		copyFrom(model);
	}

	~Field3D();

	// operator= removed, since it could look ambiguous in code. Use copyFrom() instead.
	Field3D& operator=(const Field3D &) = delete;

	// Copy all values from source.
	void copyFrom(const Field3D &source);

	// Swap 2 fields.
	void swapWith(Field3D &field);

	// Element access.

	num_t& operator()(idx_t x, idx_t y, idx_t z){
		return data[get1DIndex(x, y, z)];
	}

	const num_t& operator()(idx_t x, idx_t y, idx_t z) const{
		return data[get1DIndex(x, y, z)];
	}

	idx_t XSize() const { return x_size; }
	idx_t YSize() const { return y_size; }
	idx_t ZSize() const { return z_size; }

	idx_t LastXIndex() const { return x_size-1; }
	idx_t LastYIndex() const { return y_size-1; }
	idx_t LastZIndex() const { return z_size-1; }

private:
	num_t *data = nullptr;

	// Field dimensions.
	idx_t x_size, y_size, z_size;

	// Allocate memory for 3D field.
	void initialize();

	// Deallocate data.
	void free();

	// Overwrite all data with zeros.
	void fillWithZeros();

	// Helpers.

	bool areDimensionsEqual(const Field3D &field) const;

	idx_t dataByteSize() const { return sizeof(num_t) * x_size * y_size * z_size; }

	idx_t trimmedX(idx_t x) const { return (x < 0) ? (x_size + x) : x; }
	idx_t trimmedY(idx_t y) const { return (y < 0) ? (y_size + y) : y; }
	idx_t trimmedZ(idx_t z) const { return (z < 0) ? (z_size + z) : z; }

	idx_t get1DIndex(idx_t x, idx_t y, idx_t z) const;
};
