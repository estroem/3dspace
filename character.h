#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include <cmath>
#include <fstream>
#include <vector>
#include <stdio.h>
#include "drawableobject.h"
#include "mathvector.h"
#include "box.h"
#include "GL\glut.h"

using namespace std;

class Character : public DrawableObject {
private:
	static int nextId;
protected:
	void updatePlayerLocation();
public:
	int id;
	float posX;
	float posY;
	float posZ;
	float angleH;
	float angleV;
	float turningSpeed;
	float movingSpeed;
	bool goF, goB, goL, goR;
	bool turnU, turnD, turnL, turnR;

	Character() : posX(0), posY(0), posZ(0), angleH(0), angleV(0), turningSpeed(4), movingSpeed(0.4) {
		pos.x = 0;
		pos.y = 0;
		pos.z = 0;
		goF = goB = goL = goR = false;
		turnU = turnD = turnL = turnR = false;

		this->id = Character::nextId++;
	}

	Character(float x, float y, float z) : angleH(0), angleV(0), turningSpeed(4), movingSpeed(0.4) {
		pos.x = x;
		pos.y = y;
		pos.z = z;
		goF = goB = goL = goR = false;
		turnU = turnD = turnL = turnR = false;

		this->id = Character::nextId++;
	}

	void jump();

	virtual void update() = 0;
	virtual void draw() = 0;
};

#endif