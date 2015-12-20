#ifndef __FLOOR_H__
#define __FLOOR_H__

#include "GL\glut.h"
#include "mathvector.h"
#include "mathplane.h"

class Floor {
private:
	static int nextId;
public:
	int id;
	MathVector corner1;
	MathVector corner2;
	MathVector corner3;
	MathVector corner4;
	float colorR;
	float colorG;
	float colorB;

	Floor() {
		this->colorR = this->colorG = this->colorB = 0;
		this->id = Floor::nextId++;
	}

	Floor(MathVector corner1, MathVector corner2, MathVector corner3, MathVector corner4) : corner1(corner1), corner2(corner2), corner3(corner3), corner4(corner4) {
		this->colorR = this->colorG = this->colorB = 0;
		this->id = Floor::nextId++;
	}

	void draw();
	void setColor(float red, float green, float blue);
	MathPlane getPlane();
	bool getYAt(float *y, float x, float z);
};

#endif