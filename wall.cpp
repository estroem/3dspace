#include "wall.h"
#include "globals.cpp"
#include "triangle.h"

int Wall::nextId = 0;

MathPlane Wall::getPlane() {
	return MathPlane(start, end, MathVector(start.x, start.y + height, start.z));
}

bool Wall::lineCrosses(MathVector start, MathVector dir) {
	MathVector res;
	return lineCrosses(&res, start, dir, false);
}

bool Wall::lineCrosses(MathVector *result, MathVector start, MathVector dir) {
	return lineCrosses(result, start, dir, false);
}

bool Wall::lineCrosses(MathVector *result, MathVector start, MathVector dir, bool eps) {
	Triangle t1(this->start, end, MathVector(this->start.x, this->start.y + height, this->start.z));
	Triangle t2(end, MathVector(this->start.x, this->start.y + height, this->start.z), MathVector(end.x, end.y + height, end.z));

	if(t1.lineCrosses(result, start, dir, eps) || t2.lineCrosses(result, start, dir, eps)) {
		return true;
	} else {
		return false;
	}
}

bool Wall::intersection(MathVector *result, MathVector start, MathVector dir, bool eps) {
	Triangle t1(this->start, end, MathVector(this->start.x, this->start.y + height, this->start.z));
	Triangle t2(end, MathVector(this->start.x, this->start.y + height, this->start.z), MathVector(end.x, end.y + height, end.z));

	if(t1.intersection(result, start, dir, eps) || t2.intersection(result, start, dir, eps)) {
		return true;
	} else {
		return false;
	}
}

// Draw the wall
void Wall::draw() {
	glBegin(GL_QUADS);
	glColor3f(this->colorR, this->colorG, this->colorB);

	glVertex3f(start.x, start.y, start.z);
	glVertex3f(end.x, end.y, end.z);
	glVertex3f(end.x, end.y + height, end.z);
	glVertex3f(start.x, start.y + height, start.z);

	glEnd();
}

void Wall::setColor(float red, float green, float blue) {
	this->colorR = red;
	this->colorG = green;
	this->colorB = blue;
}