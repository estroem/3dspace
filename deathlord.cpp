#include "deathlord.h"
#include "globals.cpp"

void Deathlord::update() {
	if(this == player) Character::updatePlayerLocation(); // change this later to updating position of player

	if(this->id == 1) {
		if(this->cycle) // cycle switches between true and false
			this->pos.x += 0.1;
		else
			this->pos.x -= 0.1;
		if(this->pos.x >= 1)
			this->cycle = false;
		else if(this->pos.x <= -1)
			this->cycle = true;
	}

}

void Deathlord::draw() {
	DrawableObject::draw();
}