#include "mathvector.h"

MathVector MathVector::operator+ (MathVector v) {
	MathVector result;
	result.x = this->x + v.x;
	result.y = this->y + v.y;
	result.z = this->z + v.z;
	return result;
}

MathVector MathVector::operator+ (float t) {
	MathVector result;
	result.x = this->x + t;
	result.y = this->y + t;
	result.z = this->z + t;
	return result;
}

MathVector MathVector::operator- (MathVector v) {
	MathVector result;
	result.x = this->x - v.x;
	result.y = this->y - v.y;
	result.z = this->z - v.z;
	return result;
}

MathVector MathVector::operator- (float t) {
	MathVector result;
	result.x = this->x - t;
	result.y = this->y - t;
	result.z = this->z - t;
	return result;
}

MathVector MathVector::operator* (MathVector v) { // Cross product
	MathVector result;
	result.x = this->y * v.z - this->z * v.y;
	result.y = -(this->x * v.z - this->z * v.x);
	result.z = this->x * v.y - this->y * v.x;
	return result;
}

MathVector MathVector::operator* (float t) {
	MathVector result;
	result.x = this->x * t;
	result.y = this->y * t;
	result.z = this->z * t;
	return result;
}

float MathVector::length() {
	return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
}

float MathVector::cosAngle(MathVector v) {
	return (this->x * v.x + this->y * v.y + this->z * v.z) / (this->length() * v.length());
}

void MathVector::setValues(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}