#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "globals.cpp"
#include "drawableobject.h"
#include "box.h"
#include "floor.h"

#define STEP 0.5
#define EPS 0.0001
#define GRAVITY 0.02

using namespace std;

MathVector DrawableObject::getLowestPoint() {
	MathVector zero;
	if(model.size() == 0) return zero;

	unsigned int minPointIndex = 0; // do this when loading model instead
	for(unsigned int i = 1; i < model.size(); ++i) {
		if(model[i].y < model[minPointIndex].y) minPointIndex = i;
	}

	return model[minPointIndex];
}

MathVector DrawableObject::getHighestPoint() {
	MathVector p;
	return p;
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
	ySpeed = 0;

	return true;
}
/*
void DrawableObject::updatePhysics() {
	MathVector ySpeedVector(0, ySpeed, 0);

	MathVector lowestPoint = getLowestPoint();

	if(ySpeed == 0) { // maybe not compare floats like this?

		unsigned int possibleFloor = 0;
		float possibleFloorY = 0;
		bool found = false;

		for(unsigned int i = 0; i < floors.size(); ++i) {
			float floorY;
			if(floors[i]->getYAt(&floorY, pos.x, pos.z) &&
					((floorY == lowestPoint.y) || (floorY - lowestPoint.y > 0 && fabs(floorY - lowestPoint.y) < STEP) || (floorY - lowestPoint.y >= -EPS))) {
				if(!found || floorY > possibleFloorY) {
					found = true;
					possibleFloor = i;
					possibleFloorY = floorY;
				}
			}
		}
	}

	else if(ySpeed < 0) {
		
	}

	else {
	
	}

}

*/
void DrawableObject::updatePhysics() {
	if(!floor) { // not on the floor
		ySpeed -= GRAVITY;
		MathVector ySpeedVector(0, ySpeed, 0);

		for(unsigned int i = 0; i < floors.size(); ++i) {
			if(floors[i]->getPlane().lineCrosses(pos, ySpeedVector)) {
				floor = floors[i];
				snapToFloor();
				return;
			}
		}

		// not hitting a floor
		pos = pos + ySpeedVector;
	} else {
		float floorY;
		if(floor->getYAt(&floorY, pos.x, pos.z) ) {
			snapToFloor();
		}

		else {
			unsigned int possibleFloor = 0;
			float possibleFloorY = 0;
			bool found = false;

			for(unsigned int i = 0; i < floors.size(); ++i) {
				float otherFloorY;
				if(floors[i]->getYAt(&otherFloorY, pos.x, pos.z) &&
						((otherFloorY == floorY) || (otherFloorY - floorY > 0 && fabs(otherFloorY - floorY) < STEP) || (otherFloorY - floorY >= -EPS))) {
					if(!found || otherFloorY > possibleFloorY) {
						found = true;
						possibleFloor = i;
						possibleFloorY = otherFloorY;
					}
				}
			}

			if(found) {
				floor = floors[possibleFloor];
				snapToFloor();
				return;
			}

			else {
				floor = 0;
			}
		}
	}
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