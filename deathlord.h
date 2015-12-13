#ifndef __DEATHLORD_H__
#define __DEATHLORD_H__

#include "character.h"

class Deathlord : public Character {
public:
	float cycle;

	Deathlord() : Character(), cycle(0) {}
	Deathlord(float posX, float posY, float posZ) : Character(posX, posY, posZ), cycle(0) {}
	void update();
	void draw();
};

#endif