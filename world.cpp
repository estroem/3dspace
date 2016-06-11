#include "world.h"
/*
void World::setCharacters(vector<Character *>* characters) {
	this->characters = *characters;
}

void World::setWalls(vector<Wall *>* walls) {
	this->walls = *walls;
}*/

const vector<Wall *>* World::getAllWalls() {
	return &walls;
}

const vector<Floor *>* World::getAllFloors() {
	return &floors;
}

const vector<Object *>* World::getAllObjects() {
	return &objects;
}

const vector<Character *>* World::getAllCharacters() {
	return &characters;
}