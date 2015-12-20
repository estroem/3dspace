#ifndef __MATHPLANE_H__
#define __MATHPLANE_H__

#include "mathvector.h"

class MathPlane {
public:
	MathVector point;
	MathVector normal;

	MathPlane::MathPlane(MathVector p1, MathVector p2, MathVector p3) {
		point = p1;
		normal = (p2 - p1) * (p3 - p1); // Cross product of p1->p2 and p1->p3
	}
	
	bool getXAt(float *x, float y, float z);
	bool getYAt(float *y, float x, float z);
	bool getZAt(float *z, float x, float y);
	bool lineCrosses(MathVector start, MathVector dir);
};

#endif