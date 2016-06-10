#include "line.h"

#include <stdio.h>

#define EPS 0.001

bool Line::intersects(Line other) {
	float value = other.start.z-start.z-(dir.z-(other.dir.z/other.dir.x)*dir.x)/(dir.y-(other.dir.y/other.dir.x)*dir.x)*(other.start.y-start.y);
	
	printf("%f %f %f\n", other.dir.x, -dir.x, (start.x - other.start.x));
	printf("%f %f %f\n", 0.0f, -dir.y + dir.x*(other.dir.y/other.dir.x), (start.y-other.start.y)-(other.dir.y/other.dir.x)*(start.x-other.start.x));

	float k = other.dir.y/other.dir.x;
	float k2 = other.dir.z/other.dir.x;

	printf("%f %f %f\n", 0.0f, -dir.z+k2*dir.x, (start.z-other.start.z)-k2*(start.x-other.start.x));

	printf("%f %f %f\n", 0.0f, 0.0f, (start.z-other.start.z)-k2*(start.x-other.start.x)-((start.y-other.start.y)-k*(start.x-other.start.x))*(-dir.z+k2*dir.x)/(-dir.y+k*dir.x));

	value = (start.z-other.start.z)-k2*(start.x-other.start.x)-((start.y-other.start.y)-k*(start.x-other.start.x))*(-dir.z+k2*dir.x)/(-dir.y+k*dir.x);

	//if(other.dir.x == 0)\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

	return (value <= EPS && value >= -EPS);
}

bool Line::intersects(MathVector start, MathVector dir) {
	return intersects(Line(start, dir));
}