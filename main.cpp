/*
3D Environment, created August 2012 and onwards by Eirik Strøm

Most of the initial code was written in late August/early September of 2012. -- 25.12.2014
 */

#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <fstream>
#include "GL/glut.h"

#include "keypress.h"
#include "mathvector.h"
#include "point.h"
#include "box.h"
#include "wall.h"
#include "floor.h"
#include "character.h"
#include "object.h"
#include "globals.cpp"
#include "deathlord.h"

using namespace std;

int r = 0;
float fps = 1000;
float gravity = 0.005;
float floorHeight = -5;
float ceilingHeight = 5;
vector<Wall *> walls;
vector<Floor *> floors;
vector<Object *> objects;
vector<Character *> characters;
Character *player;

//Initializes 3D rendering
void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL); //Enable color
	glClearColor(0.7f, 0.9f, 1.0f, 1.0f); //Change the background to sky blue
}
 
//Called when the window is resized
void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)w / (double)h, 0.4, 200.0);
}

// Updates the position of all the objects
void updateObjects() {
	vector<Object *>::iterator i;

	for(i = objects.begin(); i < objects.end(); ++i) {
		if((*i)->fixed != true) {
			(*i)->update();
		}
	}
}

// Updates the position of all the characters
void updateCharacters() {
	vector<Character *>::iterator i;

	for(i = characters.begin(); i < characters.end(); ++i) {
		(*i)->update();
	}
}

// Draws the walls
void drawWalls() {
	vector<Wall *>::iterator i;

	for(i = walls.begin(); i < walls.end(); ++i) {
		(*i)->draw();
	}
}

// Draws the floors
void drawFloors() {
	vector<Floor *>::iterator i;

	for(i = floors.begin(); i < floors.end(); ++i) {
		(*i)->draw();
	}
}

// Draws all the objects
void drawObjects() {
	vector<Object *>::iterator i;

	for(i = objects.begin(); i < objects.end(); ++i) {
		(*i)->draw();
	}
}

// Draws all the characters
void drawCharacters() {
	vector<Character *>::iterator i;

	for(i = characters.begin(); i < characters.end(); ++i) {
		(*i)->draw();
	}
}

// Draw the 3D scene
void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glRotatef(player->angleV, 1.0f, 0.0f, 0.0f); //Rotate the camera
	glRotatef(player->angleH, 0.0f, 1.0f, 0.0f); //Rotate the camera
	glTranslatef(-player->pos.x, -player->pos.y, -player->pos.z); //Move
	
	drawWalls();
	drawFloors();
	drawObjects();
	drawCharacters();

	glutSwapBuffers();
}

void update(int value) {
	updateObjects();
	updateCharacters();
    
    glutPostRedisplay(); //Tell GLUT that the scene has changed
    
    //Tell GLUT to call update again in 25 milliseconds
	glutTimerFunc(10, update, 0);
}

int main(int argc, char** argv) {
	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(700, 700);
	
	//Create the window
	glutCreateWindow("Room");
	initRendering();
	
	//Set handler functions
	glutDisplayFunc(drawScene);
	glutReshapeFunc(handleResize);
	glutKeyboardFunc(handleKeyPress);
	glutKeyboardUpFunc(handleKeyRelease);
	glutSpecialFunc(handleSpecialPress);
	glutSpecialUpFunc(handleSpecialRelease);
	
	player = new Deathlord;
	player->movingSpeed = 0.1;
	characters.push_back(player);

	Deathlord bob(0, 0, 0);
	bob.loadFromFile("table.dat");
	bob.movingSpeed = 2;
	bob.id = 1;
	characters.push_back(&bob);

	Object pyramid(0.0f, -4.0f, -5.0f);
	if(pyramid.loadFromFile("pyramid.dat"))
		objects.push_back(&pyramid);
	
//	Object table(0.0f, 0.0f, -15.0f);
//	table.fixed = true;
//	if(table.loadFromFile("table.dat"))
//		objects.push_back(table);
	
	Object newtable(4.6f, 14.0f, -2.5f);
	newtable.fixed = false;
	if(newtable.loadFromFile("newtable.dat"))
		objects.push_back(&newtable);
	
	Wall wall(10.0f, -10.0f, 10.0f, 0.0f);
	wall.setColor(0.5, 0.5, 1);
	walls.push_back(&wall);
	
	Wall left(-25.0f, -25.0f, -25.0f, 25.0f);
	left.setColor(1, 1, 1);
	walls.push_back(&left);
	
	Wall right(25.0f, -25.0f, 25.0f, 25.0f);
	right.setColor(1, 1, 1);
	walls.push_back(&right);
	
	Wall back(-25.0f, -25.0f, 25.0f, -25.0f);
	back.setColor(0.5, 0.5, 0.5);
	walls.push_back(&back);
	
	Wall front(-25.0f, 25.0f, 25.0f, 25.0f);
	front.setColor(0.5, 0.5, 0.5);
	walls.push_back(&front);

	Floor floor;
	floor.corner1.setValues(-25.0f, -5.0f, -25.0f);
	floor.corner2.setValues(-25.0f, -5.0f, 25.0f);
	floor.corner3.setValues(25.0f, -5.0f, 25.0f);
	floor.corner4.setValues(25.0f, -5.0f, -25.0f);
	floor.setColor(0.7f, 0.7f, 0.7f);
	floors.push_back(&floor);

	glutTimerFunc(31.25, update, 0);

	glutMainLoop();
	return 0;
}