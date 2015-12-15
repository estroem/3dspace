#include "floor.h"
#include "globals.cpp"

int Floor::nextId = 0;

// Draw the wall
void Floor::draw() {
	glBegin(GL_QUADS);
	glColor3f(this->colorR, this->colorG, this->colorB);

	glVertex3f(this->corner1.x, this->corner1.y, this->corner1.z);
	glVertex3f(this->corner2.x, this->corner2.y, this->corner2.z);
	glVertex3f(this->corner3.x, this->corner3.y, this->corner3.z);
	glVertex3f(this->corner4.x, this->corner4.y, this->corner4.z);

	glEnd();
}

void Floor::setColor(float red, float green, float blue) {
	this->colorR = red;
	this->colorG = green;
	this->colorB = blue;
}

MathPlane Floor::getPlane() {
	MathPlane plane(corner1, corner2, corner3);
	return plane;
}

float Floor::getHeightAt(float x, float z) {
	return getPlane().getYAt(x, z);
}