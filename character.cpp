#include "character.h"
#include "globals.cpp"

int Character::nextId = 0;

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
}

void Character::draw() {
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
bool Character::loadFromFile(char* filename) {
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
	for(unsigned int i=0; i<numberOfBoxes; ++i) {
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