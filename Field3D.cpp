#include "Field3D.hpp"

#include <cstring>
#include <iostream>
#include <utility>

Field3D::~Field3D(){
	free();
}

void Field3D::copyFrom(const Field3D &source){
	if(!areDimensionsEqual(source))
		throw std::invalid_argument("Cannot copy Field3D instance: dimension mismatch.");
	memcpy(this->data, source.data, dataByteSize());
}

void Field3D::swapWith(Field3D &field){
	if(!areDimensionsEqual(field))
		throw std::invalid_argument("Cannot swap Field3D instances: dimension mismatch.");
	std::swap(data, field.data);
}

void Field3D::initialize(){
	free();
	data = new num_t[XSize() * YSize() * ZSize()];
	fillWithZeros();
}

void Field3D::free(){
	if(data){
		delete[] data;
		data = nullptr;
	}
}

void Field3D::fillWithZeros(){
	memset(data, 0, dataByteSize());
}

bool Field3D::areDimensionsEqual(const Field3D &field) const {
	return x_size == field.x_size &&
	       y_size == field.y_size &&
	       z_size == field.z_size;
}

Field3D::idx_t Field3D::get1DIndex(Field3D::idx_t x, Field3D::idx_t y, Field3D::idx_t z) const{
	return trimmedX(x) + XSize()*trimmedY(y) + XSize()*YSize()*trimmedZ(z);
}
