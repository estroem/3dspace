#include "triangle.h"
#include <stdio.h>
#include <limits>

#define EPS 0.001 // used for float comparison

bool Triangle::isWithin(MathVector point) {
	Triangle t1(p1, p2, point);
	Triangle t2(p2, p3, point);
	Triangle t3(p3, p1, point);
	/*
	printf("p1 %f %f\n", p3.x, p3.z);
	printf("p2 %f %f\n", p1.x, p1.z);
	printf("point %f %f\n", point.x, point.z);
	printf("area %f\n", t3.getArea());
	*/
	return (getArea() + EPS >= t1.getArea() + t2.getArea() + t3.getArea());
}

float Triangle::getArea() {
	return ((p2 - p1) * (p3 - p1)).length() / 2;
}

MathPlane Triangle::getPlane() {
	MathPlane plane(p1, p2, p3);
	return plane;
}

bool Triangle::lineCrosses(MathVector start, MathVector dir) {
	MathVector r;
	return lineCrosses(&r, start, dir);
}

bool Triangle::lineCrosses(MathVector *result, MathVector start, MathVector dir) {
	return lineCrosses(result, start, dir, false);
}

bool Triangle::lineCrosses(MathVector *result, MathVector start, MathVector dir, bool eps) {
	MathPlane plane = getPlane();

	if(dir.dotProduct(plane.normal) == 0) return false; // parallell

	float multiplier = (plane.point - start).dotProduct(plane.normal) / dir.dotProduct(plane.normal);
	
	if(multiplier + EPS >= 0 && multiplier - EPS <= 1) {
		if(isWithin(start + (dir * multiplier))) {
			if(eps)	*result = start + (dir * multiplier) + (dir / dir.length()) * EPS;
			else    *result = start + (dir * multiplier);
			return true;
		}
	}

	return false;
}

bool Triangle::intersection(MathVector *result, MathVector start, MathVector dir, bool eps) {	MathPlane plane = getPlane();
	if(dir.dotProduct(plane.normal) == 0) return false; // parallell

	float multiplier = (plane.point - start).dotProduct(plane.normal) / dir.dotProduct(plane.normal);
	
	if(isWithin(start + (dir * multiplier))) {
		if(eps)	*result = start + (dir * multiplier) + (dir / dir.length()) * EPS;
		else    *result = start + (dir * multiplier);
		return true;
	}

	return false;
}