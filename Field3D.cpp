#include "Field3D.hpp"

#include <cstring>
#include <iostream>
#include <utility>

Field3D::~Field3D(){
	free();
}

void Field3D::copyFrom(const Field3D &source){
	memcpy(this->data, source.data, dataByteSize());
	x_size = source.x_size;
	y_size = source.y_size;
	z_size = source.z_size;
}

void Field3D::swap(Field3D &a, Field3D &b){
	std::swap(a.data, b.data);
	std::swap(a.x_size, b.x_size);
	std::swap(a.y_size, b.y_size);
	std::swap(a.z_size, b.z_size);
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

Field3D::idx_t Field3D::get1DIndex(Field3D::idx_t x, Field3D::idx_t y, Field3D::idx_t z) const{
	return trimmedX(x) + XSize()*trimmedY(y) + XSize()*YSize()*trimmedZ(z);
}
