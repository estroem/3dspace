#include "object.h"
#include "globals.cpp"

Object::Object(float x, float y, float z) : fixed(false) {
	this->pos = MathVector(x, y, z);
	this->speed = MathVector();
	this->touching = 0;
	this->touchingPoint = 0;
	this->touchingTriangle = 0;
	static int curId = 0;
	this->id = curId++;
}

// Update the position of the object
void Object::update() {
	bool collideWithTouchingTriangle = false;
	bool collision = false;
	MathVector collisionPoint = MathVector();
	MathVector speedvector = MathVector();

	if(this->speed.x != 0 || this->speed.y != 0 || this->speed.z != 0) {
		MathVector next = this->pos + this->speed; // Next position for this object
		
		vector<MathVector> previousPoints;
		int size1 = this->model.size();
		for(int i=0; i<size1; ++i) { // Loop through all the points
			MathVector thisPoint = this->pos + this->model[i];

			int size2 = previousPoints.size();
			for(int j=0; j<size2; ++j) { // Check that we haven't done this point before
				if(this->model[i].x == previousPoints[j].x && this->model[i].y == previousPoints[j].y && this->model[i].z == previousPoints[j].z) {
					previousPoints.push_back(this->model[i]);
					goto nextPoint;
				}
			}

			// Floor
			if(next.y + this->model[i].y <= floorHeight) {
				collision = true;
				collisionPoint = this->pos;
				collisionPoint.y = floorHeight - this->model[i].y;
				speedvector = this->speed;
				speedvector.y = 0;
				break;
			}

			// Go through each of the other objects
			int size3 = objects.size();
			for(int k=0; k<size3; ++k) {
				// If it's the same object, continue
				if(this->id == objects[k]->id) {
					continue;
				}
				
				// Check if the distance is small enough
				float distance = sqrt(pow(objects[k]->pos.x - next.x, 2) + pow(objects[k]->pos.y - next.y, 2) + pow(objects[k]->pos.z - next.z, 2));

				if(distance < this->maxRadius + objects[k]->maxRadius) {
					int size4 = objects[k]->model.size();
					for(int l=0; l<size4-1; l+=3) { // Loop through the triangles of the other object
						//Fucking math
						MathVector vector1, vector2, dirvec;
						vector1 = objects[k]->model[l+1] - objects[k]->model[l]; // Two of the edges of the triangle
						vector2 = objects[k]->model[l+2] - objects[k]->model[l];
						dirvec = vector1 * vector2; // This is the surface normal of the plane of the triangle
						
						MathVector otherPoint, otherPoint2, otherPoint3;
						otherPoint = objects[k]->pos + objects[k]->model[l]; // Each corner of the triangle
						otherPoint2 = objects[k]->pos + objects[k]->model[l+1];
						otherPoint3 = objects[k]->pos + objects[k]->model[l+2];
						
						float t = (dirvec.x*(otherPoint.x - thisPoint.x) + dirvec.y*(otherPoint.y - thisPoint.y) + dirvec.z*(otherPoint.z - thisPoint.z)) / (dirvec.x*this->speed.x + dirvec.y*this->speed.y + dirvec.z*this->speed.z); // I have no idea what this does
						
						MathVector intersection = thisPoint + (this->speed * t); // Intersection between the path of thisPoint and the plane
//						if(this->id == 1 &&  objects[k]->id == 0){
//							printf("%f\n", t);
//						}

						if(this->touching > 0 && i == this->touchingPoint && objects[k]->id == this->touchingObject->id && l == this->touchingTriangle) { // Check if we are colliding with the triangle we are touching
							if(this->touching == 1) {
								if(this->speed.cosAngle(dirvec) < 0)
									continue; // We are moving away from the triangle and can continue freely
								else
									collideWithTouchingTriangle = true; // Crash
							}
							if(this->touching == 2) {
								if(this->speed.cosAngle(dirvec) > 0)
									continue;
								else
									collideWithTouchingTriangle = true;
							}
						}

						if(collideWithTouchingTriangle) {printf("OK");
							float t2 = (dirvec.x*(otherPoint.x - (thisPoint.x + this->speed.x)) + dirvec.y*(otherPoint.y - (thisPoint.y + this->speed.y)) + dirvec.z*(otherPoint.z - (thisPoint.z + this->speed.z))) / (pow(dirvec.x, 2) + pow(dirvec.y, 2) + pow(dirvec.z, 2));
							
							MathVector intersection2 = this->speed + (dirvec * t2);
							speedvector = intersection2 * (this->speed.length() / intersection2.length());
							
							collisionPoint = this->pos + speedvector;
							goto collide;
						}

						if((intersection - thisPoint).length() <= this->speed.length()) { // Check if we are crossing the plane
							MathVector pointvec1, pointvec2, pointvec3, vecprod1, vecprod2, vecprod3;//if(this->id == 2) printf("%f", (intersection-thisPoint).length());printf("   ");printf("%f", this->speed.length());printf("\n");
							pointvec1 = intersection - otherPoint; // Vectors from the intersection to each corner
							pointvec2 = intersection - otherPoint2;
							pointvec3 = intersection - otherPoint3;
							
							vecprod1 = pointvec1 * pointvec2; // Vector product of pairs of the previously mentioned vectors
							vecprod2 = pointvec1 * pointvec3; // Is used to calculate the area of three triangles inside the big one
							vecprod3 = pointvec2 * pointvec3;
							
							float areaBigTriangle = dirvec.length(); // Area of the big triangle
							
							float areaTriangle1 = vecprod1.length(); // Area of each of the small triangles
							float areaTriangle2 = vecprod2.length(); // If the sum of these is greater than the big triangle,
							float areaTriangle3 = vecprod3.length(); // the intersection is outside the big triangle
							
							if(areaTriangle1 + areaTriangle2 + areaTriangle3 <= areaBigTriangle) {
								float angle = (thisPoint - intersection).cosAngle(dirvec); // Returns the cosine of the angle between the trajectory and the normal
								if(angle > 0) // Determine what side of the triangle we are on
									this->touching = 1;
								else
									this->touching = 2;

								this->touchingObject = objects[k];
								this->touchingTriangle = l;
								this->touchingPoint = i;

								collision = true;

								MathVector speedFromIntersection = this->speed - (intersection - thisPoint);

						//		collisionPoint = intersection - this->model[i];
								
								float t3 = (dirvec.x*(otherPoint.x - (thisPoint.x + this->speed.x)) + dirvec.y*(otherPoint.y - (thisPoint.y + this->speed.y)) + dirvec.z*(otherPoint.z - (thisPoint.z + this->speed.z))) / (pow(dirvec.x, 2) + pow(dirvec.y, 2) + pow(dirvec.z, 2));
								MathVector intersection3 = this->speed + (dirvec * t3);
								
								MathVector distanceBetweenIntersections = intersection3 - intersection;
								
								collisionPoint = (intersection + distanceBetweenIntersections * (speedFromIntersection.length() / distanceBetweenIntersections.length())) - this->model[i];
								
								speedvector = distanceBetweenIntersections * (this->speed.length() / distanceBetweenIntersections.length());

								goto collide;
							}
						}
					}
				}
			}
		nextPoint:;
		}
	}

	/*
	for(unsigned int i=0; i<this->numberOfBoxes; ++i) { // Loop through this object's boxes
		float thisNextLeft = nextX + this->boxModel[i].left;
		float thisNextRight = nextX + this->boxModel[i].right;
		float thisNextTop = nextY + this->boxModel[i].top;
		float thisNextBottom = nextY + this->boxModel[i].bottom;
		float thisNextFront = nextZ + this->boxModel[i].front;
		float thisNextBack = nextZ + this->boxModel[i].back;
					
		// Floor
		if(this->pos.y + this->boxModel[i].bottom >= floorHeight && thisNextBottom < floorHeight) {
			collisionY = true;
			collisionPointY = floorHeight - this->boxModel[i].bottom;
		}

		int size = walls.size();
		for(int l=0; l<size; ++l) {
			float wallLeft = min(walls[l].startX, walls[l].endX);
			float wallRight = max(walls[l].startX, walls[l].endX);
			float wallFront = max(walls[l].startZ, walls[l].endZ);
			float wallBack = min(walls[l].startZ, walls[l].endZ);

			if(thisNextBack < wallFront && thisNextFront > wallBack) {
				if(this->pos.x + this->boxModel[i].left >= wallRight && thisNextLeft < wallRight) { // Moving left
					collisionX = true;
					collisionPointX = wallRight - this->boxModel[i].left;
				}

				if(this->pos.x + this->boxModel[i].right <= wallLeft && thisNextRight > wallLeft) { // Moving right
					collisionX = true;
					collisionPointX = wallLeft - this->boxModel[i].right;
				}
			}

			if(thisNextLeft < wallRight && thisNextRight > wallLeft) {
				if(this->pos.z + this->boxModel[i].back >= wallFront && thisNextBack < wallFront) { // Moving backwards
					collisionZ = true;
					collisionPointZ = wallFront - this->boxModel[i].back;
				}

				if(this->pos.z + this->boxModel[i].front <= wallBack && thisNextFront > wallBack) { // Moving forwards
					collisionZ = true;
					collisionPointZ = wallBack - this->boxModel[i].front;
				}
			}
		}

		int size2 = objects.size();
		for(int j=0; j<size2; ++j) {

			// If it's the same object, continue
			if(this->id == objects[j].id) {
				continue;
			}

			// Distance betweent the two
			float distance = sqrt(pow(objects[j].pos.x - nextX, 2) + pow(objects[j].pos.y - nextY, 2) + pow(objects[j].pos.z - nextZ, 2));

			if(distance < this->maxRadius + objects[j].maxRadius) {
				// Objects are within striking distance

				for(unsigned int k=0; k<objects[j].numberOfBoxes; ++k) { // Loop through other object's boxes
					float otherNextLeft = objects[j].pos.x + objects[j].boxModel[k].left;
					float otherNextRight = objects[j].pos.x + objects[j].boxModel[k].right;
					float otherNextTop = objects[j].pos.y + objects[j].boxModel[k].top;
					float otherNextBottom = objects[j].pos.y + objects[j].boxModel[k].bottom;
					float otherNextFront = objects[j].pos.z + objects[j].boxModel[k].front;
					float otherNextBack = objects[j].pos.z + objects[j].boxModel[k].back;
						
					// Moving left-right
					if(thisNextBack < otherNextFront && thisNextFront > otherNextBack) {
						if(thisNextBottom < otherNextTop && thisNextTop > otherNextBottom) {
									
							if(this->pos.x + this->boxModel[i].left >= objects[j].pos.x + objects[j].boxModel[k].right && thisNextLeft < otherNextRight) { // Moving left
								collisionX = true;
								collisionPointX = otherNextRight - this->boxModel[i].left;
							}

							if(this->pos.x + this->boxModel[i].right <= objects[j].pos.x + objects[j].boxModel[k].left && thisNextRight > otherNextLeft) { // Moving right
								collisionX = true;
								collisionPointX = otherNextLeft - this->boxModel[i].right;
							}
						}
					}

					// Moving up-down
					if(thisNextBack < otherNextFront && thisNextFront > otherNextBack) {
						if(thisNextLeft < otherNextRight && thisNextRight > otherNextLeft) {

							if(this->pos.y + this->boxModel[i].bottom >= objects[j].pos.y + objects[j].boxModel[k].top && thisNextBottom < otherNextTop) { // Moving down
								collisionY = true;
								collisionPointY = otherNextTop - this->boxModel[i].bottom;
							}

							if(this->pos.y + this->boxModel[i].top <= objects[j].pos.y + objects[j].boxModel[k].bottom && thisNextTop > otherNextBottom) { // Moving up
								collisionY = true;
								collisionPointY = otherNextBottom - this->boxModel[i].top;
							}
						}
					}

					// Moving forwards and backwards
					if(thisNextBottom < otherNextTop && thisNextTop > otherNextBottom) {
						if(thisNextLeft < otherNextRight && thisNextRight > otherNextLeft) {
								
							if(this->pos.z + this->boxModel[i].back >= objects[j].pos.z + objects[j].boxModel[k].front && thisNextBack < otherNextFront) { // Moving backwards
								collisionZ = true;
								collisionPointZ = otherNextFront - this->boxModel[i].back;
							}

							if(this->pos.z + this->boxModel[i].front <= objects[j].pos.z + objects[j].boxModel[k].back && thisNextFront > otherNextBack) { // Moving forwards
								collisionZ = true;
								collisionPointZ = otherNextBack - this->boxModel[i].front;
							}
						}
					}
				}
			}
		}
	}
	*/
	collide:
	if(collision) {
		this->pos = collisionPoint;
		this->speed = speedvector;
	}

	else {
		if(collideWithTouchingTriangle) {
			this->pos = collisionPoint;
			this->speed = speedvector;
		}

		else {
			this->pos.x += this->speed.x;
			this->pos.y += this->speed.y;
			this->pos.z += this->speed.z;
		}

		this->speed.y -= gravity; // This is all the code you need to simulate gravity
		
		if(this->touching > 0 && false) {
			MathVector vector1, vector2, dirvec;
			vector1 = this->touchingObject->model[this->touchingTriangle + 1] - this->touchingObject->model[this->touchingTriangle]; // Two of the edges of the triangle
			vector2 = this->touchingObject->model[this->touchingTriangle + 2] - this->touchingObject->model[this->touchingTriangle];
			dirvec = vector1 * vector2; // This is the surface normal of the plane of the triangle

			MathVector thisPoint, otherPoint, otherPoint2, otherPoint3;
			thisPoint = this->pos + this->model[this->touchingPoint];
			otherPoint = this->touchingObject->pos + this->touchingObject->model[this->touchingTriangle]; // Each corner of the triangle
			otherPoint2 = this->touchingObject->pos + this->touchingObject->model[this->touchingTriangle + 1];
			otherPoint3 = this->touchingObject->pos + this->touchingObject->model[this->touchingTriangle + 2];
		
			float t = (dirvec.x*(otherPoint.x - thisPoint.x) + dirvec.y*(otherPoint.y - thisPoint.y) + dirvec.z*(otherPoint.z - thisPoint.z)) / (dirvec.x*dirvec.x + dirvec.y*dirvec.y + dirvec.z*dirvec.z);
		
			MathVector intersection = thisPoint + (dirvec * t); // Intersection between the path of thisPoint and the plane
		
			MathVector pointvec1, pointvec2, pointvec3, vecprod1, vecprod2, vecprod3;
			pointvec1 = intersection - otherPoint; // Vectors from the intersection to each corner
			pointvec2 = intersection - otherPoint2;
			pointvec3 = intersection - otherPoint3;
		
			vecprod1 = pointvec1 * pointvec2; // Vector profuct of pairs of the previously mentioned vectors
			vecprod2 = pointvec1 * pointvec3; // Is used to calculate the area of three triangles inside the big one
			vecprod3 = pointvec2 * pointvec3;
			
			float areaOfTriangle = sqrt(pow(dirvec.x, 2) + pow(dirvec.y, 2) + pow(dirvec.z, 2)); // Area of the big triangle
			
			float areaTriangle1 = sqrt(pow(vecprod1.x, 2) + pow(vecprod1.y, 2) + pow(vecprod1.z, 2)); // Area of each of the small triangle
			float areaTriangle2 = sqrt(pow(vecprod2.x, 2) + pow(vecprod2.y, 2) + pow(vecprod2.z, 2)); // If the sum of these is greater than the big triangle,
			float areaTriangle3 = sqrt(pow(vecprod3.x, 2) + pow(vecprod3.y, 2) + pow(vecprod3.z, 2)); // the intersection is outside the big triangle
			
			if(areaTriangle1 + areaTriangle2 + areaTriangle3 > areaOfTriangle) {
				this->touching = 0;
				this->touchingObject = 0;
				this->touchingPoint = 0;
				this->touchingTriangle = 0;
			}
		}
	}
}

// Draw the object
void Object::draw() {
	glPushMatrix();
	glTranslatef(this->pos.x, this->pos.y, this->pos.z);
	glBegin(GL_TRIANGLES);
	
	glColor3f(1.0f, 0.0f, 0.0f);
	if(this->id == 1)
		glColor3f(0.6f, 0.4f, 0.2f);
	if(this->id == 0)
		glColor3f(0.0f, 0.0f, 1.0f);
	
	int size = this->model.size();
	for(int i=0; i<size; ++i) {
		glVertex3f(this->model[i].x, this->model[i].y, this->model[i].z);
	}
	glEnd();

	glBegin(GL_POINTS);
		glColor3f(0.0f, 1.0f, 0.0f);
	if(this->touchingPoint > 0)
	glVertex3f((this->pos + this->model[this->touchingPoint] + this->speed).x, (this->pos + this->model[this->touchingPoint] + this->speed).y, (this->pos + this->model[this->touchingPoint] + this->speed).z);

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
bool Object::loadFromFile(char* filename) {
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
		this->boxModel.push_back(box);
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

// Load a model from an array (not in use)
bool Object::loadModelFromArray(short* data, int size) {
	for(int i=0; i<size; i+=3) {
		MathVector point;
		point.x = data[i];
		point.y = data[i+1];
		point.z = data[i+2];
		this->model.push_back(point);
	}

	return true;
}