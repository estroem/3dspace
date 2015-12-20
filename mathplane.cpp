#include "mathplane.h"
#include "mathvector.h"

bool MathPlane::getXAt(float *x, float y, float z) {
	MathVector xAxis(1, 0, 0); // line parallell to the x-axis
	MathVector fromPoint(0, y, z);
	
	if(xAxis.dotProduct(normal) == 0) { // the plane is perpenicular to the x plane
		return false; // should obviously be changed
	}

	// intersection between the line parallell to the x-axis from (0, y, z) and the plane
	*x = (point - fromPoint).dotProduct(normal) / xAxis.dotProduct(normal);

	return true;
}

bool MathPlane::getYAt(float *y, float x, float z) {
	MathVector yAxis(0, 1, 0); // line parallell to the y-axis
	MathVector fromPoint(x, 0, z);
	
	if(yAxis.dotProduct(normal) == 0) { // the plane is perpenicular to the y plane
		return false; // should obviously be changed
	}

	// intersection between the line parallell to the y-axis from (x, 0, z) and the plane
	*y = (point - fromPoint).dotProduct(normal) / yAxis.dotProduct(normal);

	return true;
}

bool MathPlane::getZAt(float *z, float x, float y) {
	MathVector zAxis(0, 0, 1); // line parallell to the z-axis
	MathVector fromPoint(x, y, 0);
	
	if(zAxis.dotProduct(normal) == 0) { // the plane is perpenicular to the z plane
		return false; // should obviously be changed
	}

	// intersection between the line parallell to the z-axis from (x, y, 0) and the plane
	*z = (point - fromPoint).dotProduct(normal) / zAxis.dotProduct(normal);

	return true;
}

bool MathPlane::lineCrosses(MathVector start, MathVector dir) {
	if(dir.dotProduct(normal) == 0) return false; // parallell

	float multiplier = (point - start).dotProduct(normal) / dir.dotProduct(normal);

	if(multiplier >= 0 && multiplier <= 1) return true;

	return false;
}