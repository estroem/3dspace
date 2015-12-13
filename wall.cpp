#include "wall.h"
#include "globals.cpp"

Wall::Wall(float startX, float startZ, float endX, float endZ) : startX(startX), startZ(startZ), endX(endX), endZ(endZ) {
	static int curId = 0;
	this->id = curId++;
}

// Draw the wall
void Wall::draw() {
	glBegin(GL_QUADS);
	glColor3f(this->colorR, this->colorG, this->colorB);

	glVertex3f(this->startX, floorHeight, this->startZ);
	glVertex3f(this->endX, floorHeight, this->endZ);
	glVertex3f(this->endX, ceilingHeight, this->endZ);
	glVertex3f(this->startX, ceilingHeight, this->startZ);

	glEnd();
}

void Wall::setColor(float red, float green, float blue) {
	this->colorR = red;
	this->colorG = green;
	this->colorB = blue;
}