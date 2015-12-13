#ifndef __WALL_H__
#define __WALL_H__

#include "GL\glut.h"

class Wall {
public:
	int id;
	float startX;
	float startZ;
	float endX;
	float endZ;
	float colorR;
	float colorG;
	float colorB;

	Wall();
	Wall(float startX, float startZ, float endX, float endZ);
	void draw();
	void setColor(float red, float green, float blue);
};

#endif