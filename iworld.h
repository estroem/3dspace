#ifndef __IWORLD_H__
#define __IWORLD_H__

class Wall;
class Floor;
class Object;
class Character;

class IWorld {
public:
	virtual const vector<Wall *>* getAllWalls() = 0;
	virtual const vector<Floor *>* getAllFloors() = 0;
	virtual const vector<Object *>* getAllObjects() = 0;
	virtual const vector<Character *>* getAllCharacters() = 0;
};

#endif