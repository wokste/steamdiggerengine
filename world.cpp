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

void World::logic(int timeMs){
	entities->logic(timeMs);
}

void World::render(){
	map->render();
	entities->render();
}

bool World::areaHasBlocks(Vector2i px1, Vector2i px2, BlockCollisionType colType){
	return map->areaHasBlocks(px1,px2, colType);
}

bool World::areaHasEntity(Vector2i px1, Vector2i px2){
	return entities->areaHasEntity(px1,px2);
}
