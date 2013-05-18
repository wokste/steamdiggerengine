#include "world.h"
#include "entities/entity.h"
#include "map/map.h"
#include "items/itemdefmanager.h"
#include "game.h"
#include <stdlib.h>
#include "utils/confignode.h"
#include "entities/player.h"
#include "entities/projectile.h"
#include "entities/monster.h"
#include "items/block.h"

World::World(Game* newSettings) :
	game(newSettings),
	monsterSpawner(*game)
{
	//entities = new EntityList();
	map = new Map(rand(), game);
	map->generate();
}

World::~World(){
	//delete entities;
	delete map;
}

void World::logic(double time){
	for (auto& projectile : projectiles)
		projectile->logic(time);
	for (auto& player : players)
		player->logic(time);
	for (auto& monster : monsters)
		monster->logic(time);

	// == Removal of entities ==
	#define REMOVE_FROM_LIST(v,f) v.erase(std::remove_if (v.begin(), v.end(), f), v.end())

	REMOVE_FROM_LIST(monsters, [&](std::unique_ptr<Monster>& monster){
		return monster->HP <= 0;
	});
	REMOVE_FROM_LIST(projectiles, [&](std::unique_ptr<Projectile>& projectile){
		return projectile->state == ProjectileState::DeleteMe;
	});

	monsterSpawner.logic(this, time);
}

void World::render(){
	map->render();

	for (auto& player : players)
		player->render();
	for (auto& monster : monsters)
		monster->render();
	for (auto& projectile : projectiles)
		projectile->render();
}

bool World::areaHasBlocks(Vector2i px1, Vector2i px2, BlockCollisionType colType){
	return map->areaHasBlocks(px1,px2,
		[&](const Block* block){return (block != nullptr) && (block != nullptr && block->collisionType == colType);});
}

bool World::areaHasEntity(Vector2i px1, Vector2i px2){
	Vector2d px1d = Vector2::iToD(px1);
	Vector2d px2d = Vector2::iToD(px2);
	for (auto& player : players)
		if (player->isInArea(px1d, px2d)) return true;
	for (auto& monster : monsters)
		if (monster->isInArea(px1d, px2d)) return true;
	return false;
}
