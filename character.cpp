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
/*
int Character::wallCollision(int *wallInd, MathVector *result, MathVector start, MathVector dir) {
	puts("ok");
	MathVector next = start + dir;

	MathVector posRight(start.x + radius, start.y, start.z), posLeft(start.x - radius, start.y, start.z),
	           posBack(start.x, start.y, start.z + radius), posForw(start.x, start.y, start.z - radius);
	
	MathVector nextRight(next.x + radius, next.y, next.z), nextLeft(next.x - radius, next.y, next.z),
	           nextBack(next.x, next.y, next.z + radius), nextForw(next.x, next.y, next.z - radius);

	MathVector colLeft, colRight, colBack, colForw;
	bool colLeftFound = false, colRightFound = false, colBackFound = false, colForwFound = false;
	int leftWallInd, rightWallInd, backWallInd, forwWallInd;

	unsigned int numWalls = walls.size();
	for(unsigned int i = 0; i < numWalls; ++i) {
		if((start.y > walls[i]->start.y && start.y < walls[i]->start.y + height - STEP) // these two lines check that we are at the right height to crash into the wall
				|| (walls[i]->start.y > start.y && walls[i]->start.y < start.y + height && walls[i]->start.y + height > start.y + STEP)) {
			
			MathVector intersection;
			
			if(walls[i]->lineCrosses(&intersection, posRight, nextRight, true)) {
				if(!colRightFound || (intersection - posRight).length() < (colRight - posRight).length()) {
					colRightFound = true;
					colRight = intersection;
					rightWallInd = i;
				}
			}

			if(walls[i]->lineCrosses(&intersection, posLeft, nextLeft, true)) {
				if(!colLeftFound || (intersection - posLeft).length() < (colLeft - posLeft).length()) {
					colLeftFound = true;
					colLeft = intersection;
					leftWallInd = i;
				}
			}
			
			if(walls[i]->lineCrosses(&intersection, posBack, nextBack, true)) {
				if(!colBackFound || (intersection - posZ).length() < (colBack - posBack).length()) {
					colBackFound = true;
					colBack = intersection;
					backWallInd = i;
				}
			}

			if(walls[i]->lineCrosses(&intersection, posForw, nextForw, true)) {
				if(!colForwFound || (intersection - posForw).length() < (colForw - posForw).length()) {
					colForwFound = true;
					colForw = intersection;
					forwWallInd = i;
				}
			}
		}
	}
	
	MathVector endLeft(colLeft.x + radius, colLeft.y, colLeft.z);
	MathVector endRight(colRight.x - radius, colRight.y, colRight.z);
	MathVector endBack(colBack.x, colBack.y, colBack.z - radius);
	MathVector endForw(colForw.x, colForw.y, colForw.z + radius);

	if(colLeftFound && colBackFound) {
		if((colLeft - posLeft).length() < (colBack - posBack).length()) {
			*wallInd = leftWallInd;
			*result = endLeft;
			return LEFT;
		} else if((colLeft - posLeft).length() > (colBack - posBack).length()) {
			*wallInd = backWallInd;
			*result = endBack;
			return BACK;
		} else {
			*result = endLeft;
			return LB;
		}
	} else if(colLeftFound && colForwFound) {
		if((colLeft - posLeft).length() < (colForw - posForw).length()) {
			*wallInd = leftWallInd;
			*result = endLeft;
			return LEFT;
		} else if((colLeft - posLeft).length() > (colForw - posForw).length()) {
			*wallInd = forwWallInd;
			*result = endForw;
			return FORW;
		} else {
			*result = endLeft;
			return LF;
		}
	} else if(colRightFound && colBackFound) {
		if((colRight - posRight).length() < (colBack - posBack).length()) {
			*wallInd = rightWallInd;
			*result = endRight;
			return RIGHT;
		} else if((colRight - posRight).length() > (colBack - posBack).length()) {
			*wallInd = backWallInd;
			*result = endBack;
			return BACK;
		} else {
			*result = endRight;
			return RB;
		}
	} else if(colRightFound && colForwFound) {
		if((colRight - posRight).length() < (colForw - posForw).length()) {
			*wallInd = rightWallInd;
			*result = endRight;
			return RIGHT;
		} else if((colRight - posRight).length() > (colForw - posForw).length()) {
			*wallInd = forwWallInd;
			*result = endForw;
			return FORW;
		} else {
			*result = endRight;
			return RF;
		}
	} else if(colLeftFound) {
		*wallInd = leftWallInd;
		*result = endLeft;
		return LEFT;
	} else if(colRightFound) {
		*wallInd = rightWallInd;
		*result = endRight;
		return RIGHT;
	} else if(colBackFound) {
		*wallInd = backWallInd;
		*result = endBack;
		return BACK;
	} else if(colForwFound) {
		*wallInd = forwWallInd;
		*result = endForw;
		return FORW;
	} else {
		return NOHIT;
	}
}
*/

/*
bool Character::wallCollision(int *wallId, MathVector *result, MathVector start, MathVector dir) {
	MathVector next = start + dir;

	// check if cynlinder touches wall

	int hittingWallId = 0;
	MathVector hittingWallPoint;
	bool found = false;

	unsigned int numWalls = walls.size();
	for(unsigned int i = 0; i < numWalls; ++i) {
		if(walls[i]->start.x == walls[i]->end.x) {
			// special case
			puts("special case wall");
			return false;
		}

		float slope = (walls[i]->end.z - walls[i]->start.z) / (walls[i]->end.x - walls[i]->start.x);
		float zIntsect = walls[i]->start.z - walls[i]->start.x * slope;
		
		// this formula was crafted from the equations z=ax+b and (x-c)^2+(z-d)^2=r^2
		float intersect = pow(2 * ((slope * zIntsect) - next.x - (next.z * slope)), 2) - 4 * (1 + pow(slope, 2)) * (pow(next.x, 2) + pow(next.z, 2) + pow(zIntsect, 2) - (2 * next.z * zIntsect) - pow(radius, 2));
		
		if(intersect >= 0) { // must be strictly greater than or equal to 0, or we might try to get the square root of a negative number
			float a = 1 + pow(slope, 2);
			float b = 2 * (slope * zIntsect) - next.x - (slope * next.z);
			
			float x1 = (-b + sqrt(intersect)) / (2 * a);
			float x2 = (-b - sqrt(intersect)) / (2 * a);

			if(x1 > min(walls[i]->start.x, walls[i]->end.x) && x2 < max(walls[i]->start.x, walls[i]->end.x)) {
				puts("crash1");
				goto collision;
			}
		}

		if(walls[i]->lineCrosses(start, dir)) {
			puts("crash2");
			goto collision;
		}

		break;
		collision:

		/*
		   a = slope
		   b = zIntsect
		   c = start.x
		   d = start.z
		   i = dir.x
		   j = dir.z
		   r = radius

		   this formula was made from the equations z=ax+b and (x-(c+ik))^2+(z-(d+jk))^2=r^2
		

		float slopesq = pow(slope, 2);
		
		float a = (2 * dir.z * slope * dir.x - pow(dir.z, 2) - pow(slope * dir.x, 2));
		float b = 2 * (start.x * dir.x - zIntsect * dir.z * slopesq + start.z * dir.z * slopesq + start.x * dir.z * slope - slope * zIntsect * dir.x + start.z * slope * dir.x - start.x * dir.x + dir.z * zIntsect - start.z * dir.z - slopesq * start.x * dir.x + slopesq * dir.z * zIntsect - slopesq * start.z * dir.z);
		float c = (2 * start.x * start.z * slope - 2 * start.x * slope * zIntsect - pow(zIntsect, 2) + 2 * start.z * zIntsect - pow(start.z, 2) + pow(radius, 2) - slopesq * pow(start.x, 2) + slopesq * pow(radius, 2));

		if(a == 0) { puts("FATAL ERROR A == 0"); return false; }
		if(pow(b, 2) - (4 * a * c) < 0) { puts("FATAL ERROR sqrt(-1)"); return false; }
		
		float k1 = (-b + sqrt(pow(b, 2) - (4 * a * c))) / (2 * a);
		float k2 = (-b - sqrt(pow(b, 2) - (4 * a * c))) / (2 * a);

		*result = start + dir * min(k1, k2);
		*wallId = i;
		return true;
	}

	return false;
}
*/

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

	next = wallCollision(pos, next);

	/*
	if(side == LB || side == LF || side == RB || side == RF) {
		next.x = collisionPoint.x;
		next.z = collisionPoint.z;
	}
	
	else if(side == LEFT) {
		MathVector endPoint = walls[wallId]->getPlane().lineBreakEnd(MathVector(pos.x - radius, pos.y, pos.z), dir, true);
		endPoint.x += radius;

		int wallId2;
		MathVector collisionPoint2;
		int side2 = wallCollision(&wallId2, &collisionPoint2, collisionPoint, (endPoint - collisionPoint));

		if(side2 != NOHIT) {
			next.x = collisionPoint2.x;
			next.z = collisionPoint2.z;
		} else {
			next.x = endPoint.x;
			next.z = endPoint.z;
		}
	}
	
	else if(side == RIGHT) {
		MathVector endPoint = walls[wallId]->getPlane().lineBreakEnd(MathVector(pos.x + radius, pos.y, pos.z), dir, true);
		endPoint.x -= radius;

		int wallId2;
		MathVector collisionPoint2;
		int side2 = wallCollision(&wallId2, &collisionPoint2, collisionPoint, (endPoint - collisionPoint));

		if(side2 != NOHIT) {
			next.x = collisionPoint2.x;
			next.z = collisionPoint2.z;
		} else {
			next.x = endPoint.x;
			next.z = endPoint.z;
		}
	}
	
	else if(side == BACK) {
		MathVector endPoint = walls[wallId]->getPlane().lineBreakEnd(MathVector(pos.x, pos.y, pos.z + radius), dir, true);
		endPoint.z -= radius;

		int wallId2;
		MathVector collisionPoint2;
		int side2 = wallCollision(&wallId2, &collisionPoint2, collisionPoint, (endPoint - collisionPoint));

		if(side2 != NOHIT) {
			next.x = collisionPoint2.x;
			next.z = collisionPoint2.z;
		} else {
			next.x = endPoint.x;
			next.z = endPoint.z;
		}
	}
	
	else if(side == FORW) {
		MathVector endPoint = walls[wallId]->getPlane().lineBreakEnd(MathVector(pos.x, pos.y, pos.z - radius), dir, true);
		endPoint.z += radius;

		int wallId2;
		MathVector collisionPoint2;
		int side2 = wallCollision(&wallId2, &collisionPoint2, collisionPoint, (endPoint - collisionPoint));

		if(side2 != NOHIT) {
			next.x = collisionPoint2.x;
			next.z = collisionPoint2.z;
		} else {
			next.x = endPoint.x;
			next.z = endPoint.z;
		}
	}
	*/
	/*if(collisionX)
		next.x = collisionPointX;
	if(collisionZ)
		next.z = collisionPointZ;
	*/
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