#ifndef __WALL_H__
#define __WALL_H__

#include "GL\glut.h"
#include "mathplane.h"
#include "mathvector.h"

class Wall {
private:
	static int nextId;
public:
	int id;
	MathVector start;
	MathVector end;
	float height;

	float colorR;
	float colorG;
	float colorB;

	Wall() {
		id = nextId++;
	}

	Wall(MathVector start, MathVector end, float height) : height(height) {
		id = nextId++;
		this->start = start;
		this->end = end;
	}

	MathPlane getPlane();
	bool lineCrosses(MathVector start, MathVector dir);
	bool lineCrosses(MathVector *result, MathVector start, MathVector dir);
	bool lineCrosses(MathVector *result, MathVector start, MathVector dir, bool eps);
	bool intersection(MathVector *result, MathVector start, MathVector dir, bool eps = false);
	void draw();
	void setColor(float red, float green, float blue);
};

#endif