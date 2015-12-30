#include "character.h"
#include "globals.cpp"
#include "mathplane.h"

#define WALL_MARGIN 1
#define JUMP_SPEED 2
#define STEP 0.5
#define EPS 0.00001
#define GRAVITY 0.001

#define NOHIT 0
#define LEFT 1
#define RIGHT 2
#define BACK 3 // towards the camera
#define FORW 4 // away from the camera
#define LB 5 // hitting both left and back 
#define LF 6 // left forwards
#define RB 7 // right back
#define RF 8 // right forwards

int Character::nextId = 0;

int Character::findWallCollisionPoint(MathVector *result, MathVector start, MathVector dir) {
	int hittingWallId = -1;
	MathVector hittingWallPoint;

	int numWalls = walls.size();
	for(int i = 0; i < numWalls; ++i) {
		MathVector hit;
		MathPlane wallPlane = walls[i]->getPlane();
		if(wallPlane.intersection(&hit, start, dir)) {
			float angle = wallPlane.angle(dir);

			MathVector collision = start + dir * ((hit - start).length() - (radius + EPS) / sin(angle)) / dir.length();

			if((collision - start).length() <= dir.length()) {
				// check if correct height
				if((collision.y < walls[i]->start.y + walls[i]->height - STEP && collision.y  + height > walls[i]->start.y)) { // check that we are at the right height to crash into the wall
					if(walls[i]->intersection(&MathVector(), collision, wallPlane.normal)) { // improve this. also deal with corners
						if(hittingWallId == -1 || (collision - start).length() < (hittingWallPoint - start).length()) {
							hittingWallId = i;
							hittingWallPoint = collision;
						}
					}
				}
			}
		}
	}

	if(hittingWallId == -1) return -1;

	*result = hittingWallPoint;
	return hittingWallId;
}

MathVector Character::wallCollision(MathVector start, MathVector next) {
	MathVector dir = next - start;
	MathVector collisionPoint;

	int wallId = findWallCollisionPoint(&collisionPoint, start, dir);

	if(wallId != -1) {

		printf("pos "); start.print();
		printf("dir "); dir.print();
		printf("collisionpoint "); collisionPoint.print();

		Wall ghostWall = *walls[wallId];

		MathVector pointInPlane;
		ghostWall.intersection(&pointInPlane, start, ghostWall.getPlane().normal);
		MathVector translation = (start - pointInPlane) / (start - pointInPlane).length() * radius;

		ghostWall.start = ghostWall.start + translation;
		ghostWall.end = ghostWall.end + translation;

		//ghostWall.start.print();
		//ghostWall.end.print();

		MathVector breakEnd = ghostWall.getPlane().lineBreakEnd(start, dir, true);

		printf("breakend "); breakEnd.print();
		puts("k\n\n\n");
		
		int hittingWallId = -1;
		MathVector hittingWallPoint;

		int numWalls = walls.size();
		for(int i = 0; i < numWalls; ++i) {
			if(i == wallId) continue;

			MathVector newCollisionPoint;
			if(walls[i]->lineCrosses(&newCollisionPoint, collisionPoint, breakEnd, true)) {
				if(hittingWallId == -1 || (newCollisionPoint - collisionPoint).length() < (hittingWallPoint - collisionPoint).length()) {
					hittingWallId = i;
					hittingWallPoint = newCollisionPoint;
				}
			}
		}
		
		if(hittingWallId != -1) {
			return hittingWallPoint;
		}

		else {
			return breakEnd;
		}
	}

	return next;
}

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

    if(this->angleH > 360)
        this->angleH -= 360;
	if(this->angleH < 0)
		this->angleH += 360;

    if(this->angleV > 360)
        this->angleV -= 360;
	if(this->angleV < 0)
		this->angleV += 360;

	next = wallCollision(pos, next);
	
	if(!floor) { // not on the floor
		speed.y -= GRAVITY;

		for(unsigned int i = 0; i < floors.size(); ++i) {
			if(floors[i]->lineCrosses(pos, next + speed)) {
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

		MathVector lowestPoint = pos + getLowestPoint();

		unsigned int possibleFloor = 0;
		float possibleFloorY = 0;
		bool found = false;

		for(unsigned int i = 0; i < floors.size(); ++i) {
			float otherFloorY;
			if(floors[i]->getYAt(&otherFloorY, next.x, next.z) // filters out tiles out of reach horizontally and sets otherFloorY
					&& ((otherFloorY == floorY) || (otherFloorY - lowestPoint.y > 0 && fabs(otherFloorY - lowestPoint.y) < STEP) // allowing stepping up on a tile within reach
												|| (otherFloorY - lowestPoint.y < 0 && otherFloorY - lowestPoint.y >= -EPS)) // allowing some margin
					&& (!found || otherFloorY > possibleFloorY)) { // finding the highest tile
				found = true;
				possibleFloor = i;
				possibleFloorY = otherFloorY;
			}
		}

		if(found) { // moving to a different floor tile
			floor = floors[possibleFloor];
			pos = next;
			snapToFloor();
			return;
		}

		else {
			if(onCurrent) { // staying on the same tile
				pos = next;
				snapToFloor();
			} else { // falling off
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