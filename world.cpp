#include "world.h"
#include "entities/entitylist.h"
#include "entities/entity.h"
#include "map/map.h"
#include "entities/entityhandle.h"
#include "items/itemdefmanager.h"
#include "game.h"
#include <stdlib.h>
#include "utils/confignode.h"

World::World(Game* newSettings) :
	game(newSettings)
{
	entities = new EntityList();
	map = new Map(rand(), game);
	map->generate();
}

World::~World(){
	delete entities;
	delete map;
}

Entity* World::spawn(std::string type, Vector2d spawnPos){
	EntityStats * stat = nullptr;
	ConfigNode::load(type, [&] (ConfigNode& config){
		stat = EntityStats::staticLoad(*game, config);
	});
	if (stat)
		return spawn(stat,spawnPos);
	else
		return nullptr;
}

Entity* World::spawn(EntityStats* stats, Vector2d spawnPos){
	if (stats != nullptr){
		Entity* e = stats->spawn(*this, spawnPos);
		if (e != nullptr){
			entities->addEntity(e);
			e->world = this;
		}
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
