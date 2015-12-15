#include <math.h>

#include "mathplane.h"
#include "mathvector.h"

float MathPlane::getXAt(float y, float z) {
	MathVector xAxis(1, 0, 0); // line parallell to the x-axis
	MathVector fromPoint(0, y, z);
	
	if(xAxis.dotProduct(normal) == 0) { // the plane is perpenicular to the x plane
		return -22; // should obviously be changed
	}

	// intersection between the line parallell to the x-axis from (0, y, z) and the plane
	return (point - fromPoint).dotProduct(normal) / xAxis.dotProduct(normal);
}

float MathPlane::getYAt(float x, float z) {
	MathVector yAxis(0, 1, 0); // line parallell to the y-axis
	MathVector fromPoint(x, 0, z);
	
	if(yAxis.dotProduct(normal) == 0) { // the plane is perpenicular to the y plane
		return -22; // should obviously be changed
	}

	// intersection between the line parallell to the y-axis from (x, 0, z) and the plane
	return (point - fromPoint).dotProduct(normal) / yAxis.dotProduct(normal);
}

float MathPlane::getZAt(float x, float y) {
	MathVector zAxis(0, 0, 1); // line parallell to the z-axis
	MathVector fromPoint(x, y, 0);
	
	if(zAxis.dotProduct(normal) == 0) { // the plane is perpenicular to the z plane
		return -22; // should obviously be changed
	}

	// intersection between the line parallell to the z-axis from (x, y, 0) and the plane
	return (point - fromPoint).dotProduct(normal) / zAxis.dotProduct(normal);
}