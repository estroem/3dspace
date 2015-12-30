#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "globals.cpp"
#include "drawableobject.h"
#include "box.h"
#include "floor.h"

using namespace std;

MathVector DrawableObject::getLowestPoint() {
	if(model.size() == 0) return MathVector(0, 0, 0);

	unsigned int minPointIndex = 0; // do this when loading model instead
	for(unsigned int i = 1; i < model.size(); ++i) {
		if(model[i].y < model[minPointIndex].y) minPointIndex = i;
	}

	return model[minPointIndex];
}

MathVector DrawableObject::getHighestPoint() {
	if(model.size() == 0) return MathVector(0, 0, 0);

	unsigned int maxPointIndex = 0; // do this when loading model instead
	for(unsigned int i = 1; i < model.size(); ++i) {
		if(model[i].y > model[maxPointIndex].y) maxPointIndex = i;
	}

	return model[maxPointIndex];
}

// fix y position to snap to floor
bool DrawableObject::snapToFloor() {
	if(!floor) return false;
	
	if(model.size() == 0) {
		float height;
		if(!floor->getYAt(&height, pos.x, pos.z)) {
			return false; // floor is vertical???
		}

		pos.y = height;
		return true;
	}

	unsigned int minPointIndex = 0; // do this when loading model instead
	for(unsigned int i = 1; i < model.size(); ++i) {
		if(model[i].y < model[minPointIndex].y) minPointIndex = i;
	}

	float height;
	
	if(!floor->getYAt(&height, model[minPointIndex].x, model[minPointIndex].z)) {
		return false; // floor is vertical???
	}
	
	pos.y = height - model[minPointIndex].y;
	speed.y = 0;

	return true;
}

// return absolute position of eyes
MathVector DrawableObject::getAbsEyePos() {
	return pos + eyePos;
}

void DrawableObject::setEyePos(MathVector pos) {
	eyePos = pos;
}

// draw the character
void DrawableObject::draw() {
	glPushMatrix();
	glTranslatef(this->pos.x, this->pos.y, this->pos.z);
	glBegin(GL_TRIANGLES);
	
	glColor3f(1.0f, 0.0f, 0.0f);
//	if(this->id == 1)
//		glColor3f(0.6f, 0.4f, 0.2f);

	
	int size = this->model.size();
	for(int i=0; i<size; ++i) {
		glVertex3f(this->model[i].x, this->model[i].y, this->model[i].z);
	}
	glEnd();

	glBegin(GL_POINTS);
		glColor3f(0.0f, 1.0f, 0.0f);

	glEnd();
		/*
	glBegin(GL_QUADS);
	glColor3f(0.0f, 1.0f, 0.0f);
	int size2 = this->boxModel.size();
	for(int j=0; j<size2; ++j) {
		glVertex3f(this->boxModel[j].left, this->boxModel[j].top, this->boxModel[j].back);
		glVertex3f(this->boxModel[j].right, this->boxModel[j].top, this->boxModel[j].back);
		glVertex3f(this->boxModel[j].right, this->boxModel[j].top, this->boxModel[j].front);
		glVertex3f(this->boxModel[j].left, this->boxModel[j].top, this->boxModel[j].front);
		glVertex3f(this->boxModel[j].left, this->boxModel[j].bottom, this->boxModel[j].back);
		glVertex3f(this->boxModel[j].right, this->boxModel[j].bottom, this->boxModel[j].back);
		glVertex3f(this->boxModel[j].right, this->boxModel[j].bottom, this->boxModel[j].front);
		glVertex3f(this->boxModel[j].left, this->boxModel[j].bottom, this->boxModel[j].front);
	}
	glEnd();*/
	glPopMatrix();
}

// Load a model from a file
bool DrawableObject::loadFromFile(char* filename) {
	ifstream indata;
	indata.open(filename, ios::binary);

	if(!indata)
		return false;
	
	// Read headers
	int maxRadiusUnmodified;
	indata.read(reinterpret_cast<char*>(&maxRadiusUnmodified), 4);
	this->maxRadius = (maxRadiusUnmodified / 1000);
	
	indata.read(reinterpret_cast<char*>(&this->numberOfBoxes), 4);
	indata.read(reinterpret_cast<char*>(&this->numberOfTriangles), 4);

	// Read box model
	for(int i=0; i<numberOfBoxes; ++i) {
		short right, left, top, bottom, front, back;
		indata.read(reinterpret_cast<char*>(&right), 2);
		indata.read(reinterpret_cast<char*>(&left), 2);
		indata.read(reinterpret_cast<char*>(&top), 2);
		indata.read(reinterpret_cast<char*>(&bottom), 2);
		indata.read(reinterpret_cast<char*>(&front), 2);
		indata.read(reinterpret_cast<char*>(&back), 2);
		Box box((float)right/1000, (float)left/1000, (float)top/1000, (float)bottom/1000, (float)front/1000, (float)back/1000);
		//this->boxModel.push_back(box);
	}

	// Read model
	while(!indata.eof()) {
		short x, y, z;
		MathVector point;
		indata.read(reinterpret_cast<char*>(&x), 2);
		indata.read(reinterpret_cast<char*>(&y), 2);
		indata.read(reinterpret_cast<char*>(&z), 2);
		point.x = (float)x/1000;
		point.y = (float)y/1000;
		point.z = (float)z/1000;
		this->model.push_back(point);
	}

	indata.close();

	return true;
}