#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include "mathvector.h"
#include "mathplane.h"

class Triangle {
public:
	MathVector p1, p2, p3;

	Triangle() : p1(), p2(), p3() {}
	Triangle(MathVector p1, MathVector p2, MathVector p3) : p1(p1), p2(p2), p3(p3) {}

	float getArea();
	MathPlane getPlane();
	bool lineCrosses(MathVector start, MathVector dir);
	bool isWithin(MathVector point);
};

#endif