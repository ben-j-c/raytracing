#include "RTGeo.hpp"
#include <stdio.h>
#include <stdexcept>
#include <iostream>

RTVec::~RTVec() {
}

RTVec::RTVec() {
	x = 0; y = 0; z = 0;
}

RTVec::RTVec(const RTVec& toCopy) {
	x = toCopy.x;
	y = toCopy.y;
	z = toCopy.z;
}

RTVec::RTVec(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

RTVec RTVec::reflect(const RTVec& normal) {
	float c = 2.0*((*(this))*normal);
	RTVec returner(normal.x*c, normal.y*c, normal.z*c);
	return returner - *this;
}

/*
	Normalizes this vector.  If the magnitude is zero, the vector is non-normalizable.
*/
RTVec& RTVec::normalize() {
	float mag = sqrt( x*x + y*y + z*z);
	if(mag != 0){
		x /= mag;
		y /= mag;
		z /= mag;
	}

	return *this;
}

/*
	Returns a vector representing a unit length vector in direction of this.  If this vector
	is non-normalizable, an error is thrown;
*/
RTVec RTVec::norm() {
	float mag = sqrt(x*x + y * y + z * z);
	if(mag == 0)
		throw std::invalid_argument("RTVec is non-normalizable; zero magnitude vector");
	RTVec returner(x / mag, y / mag, z / mag);
	return returner;
}

RTVec RTVec::unit() {
	return this->norm();
}

RTVec& RTVec::operator+=(const RTVec& b) {
	x += b.x;
	y += b.y;
	z += b.z;
	return *this;
}

RTVec RTVec::operator+(const RTVec& b) {
	RTVec returner(x + b.x, y + b.y, z + b.z);
	return returner;
}

RTVec RTVec::operator-(const RTVec& b) {
	RTVec returner(x - b.x, y - b.y, z - b.z);
	return returner;
}

RTVec& RTVec::operator-=(const RTVec& b) {
	x -= b.x;
	y -= b.y;
	z -= b.z;
	return *this;
}

float RTVec::operator*(const RTVec& b) {
	return x*b.x + y*b.y + z*b.z;
}

RTVec RTVec::operator*(float s) {
	RTVec returner(x*s, y*s, z*s);
	return returner;
}

RTVec& RTVec::operator=(const RTVec& b) {
	x = b.x;
	y = b.y;
	z = b.z;
	return *this;
}

RTVec& RTVec::set(const RTVec& b) {
	x = b.x;
	y = b.y;
	z = b.z;
	return *this;
}

float RTVec::operator[](int index) {
	if (index == 0)
		return x;
	if (index == 1)
		return y;
	if (index == 2)
		return z;
	else
		throw std::invalid_argument("Index out of bounds on an RTVec");
}

RTVec RTVec::cross(const RTVec& b) {
	RTVec returner(
		y*b.z - z * b.y,
		z*b.x - x * b.z,
		x*b.y - b.y*x
	);
	return returner;
}

RTVec RTVec::scale(float sx, float sy, float sz) {
	RTVec returner(x*sx, y*sy, z*sz);
	return returner;
}

RTVec RTVec::scale(const RTVec& s) {
	RTVec returner(x*s.x, y*s.y, z*s.z);
	return returner;
}

RTVec RTVec::flip() {
	RTVec returner(-x, -y, -z);
	return returner;
}

RTVec RTVec::elemMult(const RTVec& s) {
	RTVec returner(x*s.x, y*s.y, z*s.z);
	return returner;
}

RTVec RTVec::clampToOne() {
	RTVec returner(fminf(x, 1), fminf(y, 1), fminf(z, 1));
	return returner;
}

RTVec RTVec::clampToOneKeepRatio() {
	float max = fmaxf(fmaxf(x, y), z);
	RTVec returner(x / max, y / max, z / max);
	return returner;
}

float RTVec::magnitude(){
	return sqrt(x*x + y*y + z*z);
}

float RTVec::magSquared(){
	return (x*x + y*y + z*z);
}