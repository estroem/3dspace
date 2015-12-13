#ifndef __GLOBALS_CPP__
#define __GLOBALS_CPP__

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
extern vector<Object *> objects;
extern vector<Wall *> walls;
extern vector<Floor *> floors;
extern vector<Character *> characters;
extern Deathlord player;
extern void update(int value);

#endif