#include "mathplane.h"
#include "mathvector.h"
#include "stdio.h"

#define PI (float)3.1415926535
#define EPS (float)0.00001

bool MathPlane::getXAt(float *x, float y, float z) {
	MathVector xAxis(1, 0, 0); // line parallell to the x-axis
	MathVector fromPoint(0, y, z);
	
	if(xAxis.dotProduct(normal) == 0) { // the plane is perpenicular to the x plane
		return false;
	}

	// intersection between the line parallell to the x-axis from (0, y, z) and the plane
	*x = (point - fromPoint).dotProduct(normal) / xAxis.dotProduct(normal);

	return true;
}

bool MathPlane::getYAt(float *y, float x, float z) {
	MathVector yAxis(0, 1, 0); // line parallell to the y-axis
	MathVector fromPoint(x, 0, z);
	
	if(yAxis.dotProduct(normal) == 0) { // the plane is perpenicular to the y plane
		return false;
	}

	// intersection between the line parallell to the y-axis from (x, 0, z) and the plane
	*y = (point - fromPoint).dotProduct(normal) / yAxis.dotProduct(normal);

	return true;
}

bool MathPlane::getZAt(float *z, float x, float y) {
	MathVector zAxis(0, 0, 1); // line parallell to the z-axis
	MathVector fromPoint(x, y, 0);
	
	if(zAxis.dotProduct(normal) == 0) { // the plane is perpenicular to the z plane
		return false;
	}

	// intersection between the line parallell to the z-axis from (x, y, 0) and the plane
	*z = (point - fromPoint).dotProduct(normal) / zAxis.dotProduct(normal);

	return true;
}

bool MathPlane::lineCrosses(MathVector start, MathVector dir) {
	MathVector r;
	return lineCrosses(&r, start, dir);
}

bool MathPlane::lineCrosses(MathVector *result, MathVector start, MathVector dir) {
	return lineCrosses(result, start, dir, false);
}

bool MathPlane::lineCrosses(MathVector *result, MathVector start, MathVector dir, bool eps) {
	if(dir.dotProduct(normal) == 0) return false; // parallell

	float multiplier = (point - start).dotProduct(normal) / dir.dotProduct(normal);

	if(multiplier >= 0 && multiplier <= 1) {
		if(eps) *result = start + dir * multiplier - (dir / dir.length()) * EPS;
		else    *result = start + dir * multiplier;
		return true;
	}

	return false;
}

bool MathPlane::intersection(MathVector *result, MathVector start, MathVector dir, bool eps) {
	if(dir.dotProduct(normal) == 0) return false; // parallell

	float multiplier = (point - start).dotProduct(normal) / dir.dotProduct(normal);

	if(eps) *result = start + dir * multiplier - (dir / dir.length()) * EPS;
	else    *result = start + dir * multiplier;

	return true;
}

float MathPlane::cosAngle(MathVector v) {
	return cos(PI/2 - acos(normal.cosAngle(v)));
}

float MathPlane::angle(MathVector v) {
	return abs(PI/2 - acos(normal.cosAngle(v)));
}

MathVector MathPlane::lineBreakEnd(MathVector start, MathVector dir, bool eps) {
	MathVector intersection;
	if(!lineCrosses(&intersection, start, dir)) {
		return start + dir;
	}

	MathVector pointOnPlane = start + normal * ((point - start).dotProduct(normal) / normal.dotProduct(normal));

	if((pointOnPlane - intersection).length() == 0) {
		if(eps) return intersection + (start - intersection) / (intersection - start).length() * EPS;
		else    return intersection;
	}

	MathVector finalPoint = (intersection + (intersection - pointOnPlane) / (pointOnPlane - intersection).length() * ((start + dir - intersection).length()));
	
	puts("breakend eps ");((start - pointOnPlane) / (pointOnPlane - start).length() * EPS).print();

	if(eps) return finalPoint + (start - pointOnPlane) / (pointOnPlane - start).length() * EPS;
	else    return finalPoint;
}