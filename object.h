#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <fstream>
#include <vector>
#include <stdio.h>
#include "drawableobject.h"
#include "mathvector.h"
#include "box.h"
#include "GL\glut.h"

using namespace std;

class Object : DrawableObject {
public:
	static int curId;
	int id;
	bool fixed;
	int touching;
	Object* touchingObject;
	int touchingTriangle;
	int touchingPoint;
	MathVector pos;
	MathVector speed;
	float maxRadius;
	unsigned int numberOfBoxes;
	unsigned int numberOfTriangles;
	vector<MathVector> model;
	vector<Box> boxModel;

//	Object();
	Object(float x, float y, float z);
	void update();
	void draw();
	bool loadFromFile(char* filename);
	bool loadModelFromArray(short* data, int size);
};

#endif