#ifndef __MATHPLANE_H__
#define __MATHPLANE_H__

#include <math.h>

#include "mathvector.h"

class MathPlane {
public:
	MathVector point;
	MathVector normal;

	MathPlane::MathPlane(MathVector p1, MathVector p2, MathVector p3) {
		point = p1;
		normal = (p2 - p1) * (p3 - p1); // Cross product of p1->p2 and p1->p3
	}
	
	float getXAt(float y, float z);
	float getYAt(float x, float z);
	float getZAt(float x, float y);
};

#endif