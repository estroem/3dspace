#ifndef __MATHVECTOR_H__
#define __MATHVECTOR_H__

#include <stdlib.h>
#include <cmath>

class MathVector {
public:
	float x;
	float y;
	float z;

	MathVector() : x(0), y(0), z(0) {}

	MathVector(float x, float y, float z) : x(x), y(y), z(z) {}
	MathVector operator+ (MathVector v);
	MathVector operator+ (float t);
	MathVector operator- (MathVector v);
	MathVector operator- (float t);
	MathVector operator* (MathVector v);
	float dotProduct(MathVector v);
	MathVector operator* (float t);
	float length();
	float cosAngle(MathVector v);
	void setValues(float x, float y, float z);
};

#endif