#include "world.h"
#include "entities/entitylist.h"
#include "entities/entity.h"
#include "map.h"
#include "entities/entityhandle.h"
#include "items/itemdefmanager.h"

World * world;

World::World(){
	entities = new EntityList();
	map = new Map(new ItemDefManager());
	map->generate(1);
}

World::~World(){
	delete entities;
	delete map;
}

Entity * World::spawn(std::string type, Vector2d spawnPos){
	return spawn(entities->load(type),spawnPos);
}

Entity * World::spawn(EntityStats* stats, Vector2d spawnPos){
	if (stats != nullptr){
		Entity * e = stats->spawn(spawnPos);
		if (e != nullptr)
			entities->addEntity(e);
		return e;
	}
	return nullptr;
}

bool World::validPos(int x1Px, int x2Px, int y1Px, int y2Px){
	return map->validPos(x1Px, x2Px, y1Px, y2Px);
}

void World::logic(int timeMs){
	entities->logic(timeMs);
}

void World::render(){
	map->render();
	entities->render();
}
