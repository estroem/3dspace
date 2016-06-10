#ifndef __LINE_H__
#define __LINE_H__

#include "mathvector.h"

class Line {
public:
	MathVector start;
	MathVector dir;

	Line() {}
	Line(MathVector start, MathVector dir) {
		this->start = start;
		this->dir = dir;
	}
	
	bool intersects(Line other);
	bool intersects(MathVector start, MathVector dir);
};

#endif