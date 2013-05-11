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
#include "entities/flyingmonster.h"

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

Player* World::spawn(PlayerStats* stats, Vector2d spawnPos){
	Player* e = stats->spawn(*this, spawnPos);
	if (e != nullptr){
		players.push_back(std::unique_ptr<Player>(e));
		e->world = this;
	}
	return e;
}

Projectile* World::spawn(ProjectileStats* stats, Vector2d spawnPos){
	Projectile* e = stats->spawn(*this, spawnPos);
	if (e != nullptr){
		projectiles.push_back(std::unique_ptr<Projectile>(e));
		e->world = this;
	}
	return e;
}

Monster* World::spawn(FlyingMonsterStats* stats, Vector2d spawnPos){
	Monster* e = stats->spawn(*this, spawnPos);
	if (e != nullptr){
		monsters.push_back(std::unique_ptr<Monster>(e));
		e->world = this;
	}
	return e;
}

void World::logic(int timeMs){
	for (auto& projectile : projectiles)
		projectile->logic(timeMs);
	for (auto& player : players)
		player->logic(timeMs);
	for (auto& monster : monsters)
		monster->logic(timeMs);

	// == Removal of entities ==

	std::remove_if (monsters.begin(), monsters.end(), [&](std::unique_ptr<Monster>& monster){
		return monster->HP <= 0;
	});
	// TODO: projectiles

	monsterSpawner.logic(this, timeMs);
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
	return map->areaHasBlocks(px1,px2, colType);
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
