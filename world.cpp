#include "world.h"
/*
void World::setCharacters(vector<Character *>* characters) {
	this->characters = *characters;
}

void World::setWalls(vector<Wall *>* walls) {
	this->walls = *walls;
}*/

const vector<Wall *>* World::getAllWalls() {
	return &this->walls;
}

const vector<Floor *>* World::getAllFloors() {
	return &this->floors;
}

const vector<Object *>* World::getAllObjects() {
	return &this->objects;
}

const vector<Character *>* World::getAllCharacters() {
	return &this->characters;
}

void World::addWall(Wall* wall) {
	this->walls.push_back(wall);
}

void World::addFloor(Floor* floor) {
	this->floors.push_back(floor);
}

void World::addObject(Object* object) {
	this->objects.push_back(object);
}

void World::addCharacter(Character* character) {
	this->characters.push_back(character);
}