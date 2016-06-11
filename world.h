#ifndef __WORLD_H__
#define __WORLD_H__

#include <vector>
#include "wall.h"
#include "floor.h"
#include "character.h"
#include "object.h"
#include "deathlord.h"
#include "iworld.h"

class World : public IWorld {
private:
	vector<Wall *> walls;
	vector<Floor *> floors;
	vector<Object *> objects;
	vector<Character *> characters;

public:
	World() {
		walls = vector<Wall *>();
	}

	World(vector<Wall *>* walls, vector<Floor *>* floors, vector<Object *>* objects, vector<Character *>* characters) {
		this->walls = *walls;
		this->floors = *floors;
		this->objects = *objects;
		this->characters = *characters;
	}
	
	//void setCharacters(vector<Character *>* characters);
	//void setWalls(vector<Wall *>* walls);
	const vector<Wall *>* getAllWalls();
	const vector<Floor *>* getAllFloors();
	const vector<Object *>* getAllObjects();
	const vector<Character *>* getAllCharacters();
};

#endif