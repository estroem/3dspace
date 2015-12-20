#ifndef __DRAWABLEOBJECT_H__
#define __DRAWABLEOBJECT_H__

#include <vector>
#include "mathvector.h"
#include "floor.h"

using namespace std;

class DrawableObject {
private:
	MathVector eyePos;
	MathVector getLowestPoint();
	MathVector getHighestPoint();
protected:
	vector<MathVector> model;
	float maxRadius;
	int numberOfBoxes;
	int numberOfTriangles;
	MathVector speed;

	bool snapToFloor();
public:
	MathVector pos;
	Floor *floor;

	DrawableObject() : floor(0), eyePos() {}
	MathVector getAbsEyePos();
	void setEyePos(MathVector pos);
	bool loadFromFile(char* filename);
	virtual void draw() = 0;
};

#endif