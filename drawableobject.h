#ifndef __DRAWABLEOBJECT_H__
#define __DRAWABLEOBJECT_H__

#include <vector>
#include "mathvector.h"
#include "floor.h"

using namespace std;

class DrawableObject {
private:
	Floor *floor;
	MathVector eyePos;
protected:
	vector<MathVector> model;
	float maxRadius;
	int numberOfBoxes;
	int numberOfTriangles;

	bool snapToFloor();
	void updatePhysics();
public:
	MathVector pos;

	DrawableObject() {}
	bool loadFromFile(char* filename);
	virtual void draw() = 0;
};

#endif