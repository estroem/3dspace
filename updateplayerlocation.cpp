#include "updateplayerlocation.h"

#define WALL_MARGIN 0.5

void updatePlayerLocation(Character * player) {
//	static bool goF, goB, goL, goR;
//	static bool turnU, turnD, turnL, turnR;

	if(player->turnL)
		player->angleH -= player->turningSpeed;
    else if(player->turnR)
		player->angleH += player->turningSpeed;

	if(player->turnU && (player->angleV - player->turningSpeed > 270 || player->angleV - player->turningSpeed < 90))
		player->angleV -= player->turningSpeed;
	else if(player->turnD && (player->angleV + player->turningSpeed > 270 || player->angleV + player->turningSpeed < 90))
		player->angleV += player->turningSpeed;

	MathVector next = player->pos;
	
	if(player->goF) {
		next.x += player->movingSpeed * sin(player->angleH*3.14159/180); /*cos(player->angleV*3.14159/180) */
		next.z -= player->movingSpeed * cos(player->angleH*3.14159/180); /*cos(player->angleV*3.14159/180) */
//		next.y -= sin(angleV*3.14159/180) * player->movingSpeed;
	}
	else if(player->goB) {
		next.x -= player->movingSpeed * sin(player->angleH*3.14159/180); /*cos(player->angleV*3.14159/180) */
		next.z += player->movingSpeed * cos(player->angleH*3.14159/180); /*cos(player->angleV*3.14159/180) */
//		next.y += sin(angleV*3.14159/180) * player->movingSpeed;
	}
	if(player->goL) {
		next.x -= player->movingSpeed * cos(player->angleH*3.14159/180);
		next.z -= player->movingSpeed * sin(player->angleH*3.14159/180);
	}
	else if(player->goR) {
		next.x += player->movingSpeed * cos(player->angleH*3.14159/180);
		next.z += player->movingSpeed * sin(player->angleH*3.14159/180);
	}
	
	player->pos = next;

	bool collisionX = false;
	bool collisionZ = false;
	float collisionPointX = 0;
	float collisionPointZ = 0;

	int size = walls.size();
	for(int i=0; i<size; ++i) {
		if(player->pos.z > min(walls[i]->startZ, walls[i]->endZ) && player->pos.z < max(walls[i]->startZ, walls[i]->endZ)) {
			if(player->pos.x > walls[i]->startX != next.x + WALL_MARGIN > walls[i]->startX) {
				collisionX = true;
				collisionPointX = walls[i]->startX - WALL_MARGIN;
			}
			if(player->pos.x < walls[i]->startX != next.x - WALL_MARGIN < walls[i]->startX) {
				collisionX = true;
				collisionPointX = walls[i]->startX + WALL_MARGIN;
			}
		}

		if(player->pos.x > min(walls[i]->startX, walls[i]->endX) && player->pos.x < max(walls[i]->startX, walls[i]->endX)) {
			if(player->pos.z > walls[i]->startZ != next.z + WALL_MARGIN > walls[i]->startZ) {
				collisionZ = true;
				collisionPointZ = walls[i]->startZ - WALL_MARGIN;
			}
			if(player->pos.z > walls[i]->startZ != next.z - WALL_MARGIN > walls[i]->startZ) {
				collisionZ = true;
				collisionPointZ = walls[i]->startZ + WALL_MARGIN;
			}
		}
	}
	
	if(collisionX)
		player->pos.x = collisionPointX;
	else
		player->pos.x = next.x;
	if(collisionZ)
		player->pos.z = collisionPointZ;
	else
		player->pos.z = next.z;
	
    if(player->angleH > 360)
        player->angleH -= 360;
	if(player->angleH < 0)
		player->angleH += 360;

    if(player->angleV > 360)
        player->angleV -= 360;
	if(player->angleV < 0)
		player->angleV += 360;
}