#include "character.h"
#include "globals.cpp"

#define WALL_MARGIN 1
#define JUMP_SPEED 2
#define STEP 0.5
#define EPS 0.0001
#define GRAVITY 0.02

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
	
	bool collisionX = false;
	bool collisionZ = false;
	float collisionPointX = 0;
	float collisionPointZ = 0;

	int size = walls.size();
	for(int i=0; i<size; ++i) {
		if(next.z > min(walls[i]->startZ, walls[i]->endZ) && next.z < max(walls[i]->startZ, walls[i]->endZ)) {
			if(next.x > walls[i]->startX != next.x + WALL_MARGIN > walls[i]->startX) {
				collisionX = true;
				collisionPointX = walls[i]->startX - WALL_MARGIN;
			}
			if(next.x < walls[i]->startX != next.x - WALL_MARGIN < walls[i]->startX) {
				collisionX = true;
				collisionPointX = walls[i]->startX + WALL_MARGIN;
			}
		}

		if(next.x > min(walls[i]->startX, walls[i]->endX) && next.x < max(walls[i]->startX, walls[i]->endX)) {
			if(next.z > walls[i]->startZ != next.z + WALL_MARGIN > walls[i]->startZ) {
				collisionZ = true;
				collisionPointZ = walls[i]->startZ - WALL_MARGIN;
			}
			if(next.z > walls[i]->startZ != next.z - WALL_MARGIN > walls[i]->startZ) {
				collisionZ = true;
				collisionPointZ = walls[i]->startZ + WALL_MARGIN;
			}
		}
	}
	
	if(collisionX)
		next.x = collisionPointX;
	if(collisionZ)
		next.z = collisionPointZ;
	
    if(this->angleH > 360)
        this->angleH -= 360;
	if(this->angleH < 0)
		this->angleH += 360;

    if(this->angleV > 360)
        this->angleV -= 360;
	if(this->angleV < 0)
		this->angleV += 360;
	
	if(!floor) { // not on the floor
		speed.y -= GRAVITY;

		for(unsigned int i = 0; i < floors.size(); ++i) {
			if(floors[i]->getPlane().lineCrosses(next, speed)) {
				floor = floors[i];
				pos = next;
				snapToFloor();
				return;
			}
		}

		// not hitting a floor
		pos = next + speed;
	}
	
	else {
		float floorY;
		bool onCurrent = floor->getYAt(&floorY, next.x, next.z);

		unsigned int possibleFloor = 0;
		float possibleFloorY = 0;
		bool found = false;

		for(unsigned int i = 0; i < floors.size(); ++i) {
			float otherFloorY;
			if(floors[i]->getYAt(&otherFloorY, next.x, next.z) &&
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
			pos = next;
			snapToFloor();
			return;
		}

		else {
			if(onCurrent) {
				pos = next;
				snapToFloor();
			} else {
				floor = 0;
			}
		}
	}
}

void Character::jump() {
	if(floor) {
		floor = 0;
		speed.y = JUMP_SPEED;
	}
}