#ifndef __POINT_H__
#define __POINT_H__

class Point {
public:
	float x;
	float y;
	float z;

	Point() {}
	Point(float x, float y, float z) : x(x), y(y), z(z) {}
};

#endif