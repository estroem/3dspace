#include "character.h"
#include "globals.cpp"

#define WALL_MARGIN 1

int Character::nextId = 0;

// code for updating location for a player controlled by the keyboard
void Character::updatePlayerLocation() {
	if(this->turnL)
		this->angleH -= this->turningSpeed;
    else if(this->turnR)
		this->angleH += this->turningSpeed;

	if(this->turnU && (this->angleV - this->turningSpeed > 270 || this->angleV - this->turningSpeed < 90))
		this->angleV -= this->turningSpeed;
	else if(this->turnD && (this->angleV + this->turningSpeed > 270 || this->angleV + this->turningSpeed < 90))
		this->angleV += this->turningSpeed;

	MathVector next = this->pos;
	
	if(this->goF) {
		next.x += this->movingSpeed * sin(this->angleH*3.14159/180); /*cos(this->angleV*3.14159/180) */
		next.z -= this->movingSpeed * cos(this->angleH*3.14159/180); /*cos(this->angleV*3.14159/180) */
//		next.y -= sin(angleV*3.14159/180) * this->movingSpeed;
	}
	else if(this->goB) {
		next.x -= this->movingSpeed * sin(this->angleH*3.14159/180); /*cos(this->angleV*3.14159/180) */
		next.z += this->movingSpeed * cos(this->angleH*3.14159/180); /*cos(this->angleV*3.14159/180) */
//		next.y += sin(angleV*3.14159/180) * this->movingSpeed;
	}
	if(this->goL) {
		next.x -= this->movingSpeed * cos(this->angleH*3.14159/180);
		next.z -= this->movingSpeed * sin(this->angleH*3.14159/180);
	}
	else if(this->goR) {
		next.x += this->movingSpeed * cos(this->angleH*3.14159/180);
		next.z += this->movingSpeed * sin(this->angleH*3.14159/180);
	}
	
	this->pos = next;
	
	bool collisionX = false;
	bool collisionZ = false;
	float collisionPointX = 0;
	float collisionPointZ = 0;

	int size = walls.size();
	for(int i=0; i<size; ++i) {
		if(this->pos.z > min(walls[i]->startZ, walls[i]->endZ) && this->pos.z < max(walls[i]->startZ, walls[i]->endZ)) {
			if(this->pos.x > walls[i]->startX != next.x + WALL_MARGIN > walls[i]->startX) {
				collisionX = true;
				collisionPointX = walls[i]->startX - WALL_MARGIN;
			}
			if(this->pos.x < walls[i]->startX != next.x - WALL_MARGIN < walls[i]->startX) {
				collisionX = true;
				collisionPointX = walls[i]->startX + WALL_MARGIN;
			}
		}

		if(this->pos.x > min(walls[i]->startX, walls[i]->endX) && this->pos.x < max(walls[i]->startX, walls[i]->endX)) {
			if(this->pos.z > walls[i]->startZ != next.z + WALL_MARGIN > walls[i]->startZ) {
				collisionZ = true;
				collisionPointZ = walls[i]->startZ - WALL_MARGIN;
			}
			if(this->pos.z > walls[i]->startZ != next.z - WALL_MARGIN > walls[i]->startZ) {
				collisionZ = true;
				collisionPointZ = walls[i]->startZ + WALL_MARGIN;
			}
		}
	}
	
	if(collisionX)
		this->pos.x = collisionPointX;
	else
		this->pos.x = next.x;
	if(collisionZ)
		this->pos.z = collisionPointZ;
	else
		this->pos.z = next.z;
	
    if(this->angleH > 360)
        this->angleH -= 360;
	if(this->angleH < 0)
		this->angleH += 360;

    if(this->angleV > 360)
        this->angleV -= 360;
	if(this->angleV < 0)
		this->angleV += 360;

	DrawableObject::updatePhysics();
}