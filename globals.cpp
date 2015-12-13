#ifndef __GLOBALS_CPP__
#define __GLOBALS_CPP__

#define WALL_MARGIN 0.4

#include <vector>
#include "wall.h"
#include "floor.h"
#include "object.h"
#include "character.h"
#include "deathlord.h"

using namespace std;

extern float fps;
extern float gravity;
extern float floorHeight;
extern float ceilingHeight;
extern vector<Object *> objects; // all the objects
extern vector<Wall *> walls; // all the walls
extern vector<Floor *> floors; // all the floors
extern vector<Character *> characters; // all characters
extern Character *player; // the player
extern void update(int value);

#endif