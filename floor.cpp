#include "floor.h"
#include "globals.cpp"
#include "triangle.h"

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

// returns false if x and z are outside floor tile or floor is somehow vertical
bool Floor::getYAt(float *y, float x, float z) {
	Triangle t1(corner1, corner2, corner3);
	Triangle t2(corner1, corner4, corner3);

	float yTemp;
	if(!getPlane().getYAt(&yTemp, x, z)) {
		return false;
	}

	//printf("%f\n", yTemp);

	MathVector point(x, yTemp, z);

	if(t1.isWithin(point) || t2.isWithin(point)) {
		*y = yTemp;
		return true;
	} else {
		return false;
	}
}