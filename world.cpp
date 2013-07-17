#include "world.h"
#include "entities/entity.h"
#include "map/map.h"
#include "items/itemdefmanager.h"
#include "game.h"
#include <stdlib.h>
#include "utils/confignode.h"
#include "utils/skybox.h"
#include "entities/player.h"
#include "entities/projectile.h"
#include "entities/monster.h"
#include "map/blocktype.h"
#include <SFML/Graphics/Color.hpp>
#include <iostream>

World::World()
{
	map.reset(new Map(rand()));
	map->generate();
	monsterSpawner.reset(new MonsterSpawner());
	skybox.reset(new Skybox());
}

World::~World(){
}

void World::logic(double time){
	map->logic(time);

	for (auto& projectile : projectiles)
		projectile->logic(time);
	for (auto& player : players)
		player->logic(time);
	for (auto& monster : monsters)
		monster->logic(time);

	// == Removal of entities ==
	#define REMOVE_FROM_LIST(v,f) v.erase(std::remove_if (v.begin(), v.end(), f), v.end())

	REMOVE_FROM_LIST(monsters, [&](std::unique_ptr<Monster>& monster){
		return monster->HP <= 0 || std::none_of(players.begin(), players.end(), [&monster](std::unique_ptr<Player>& player){
			return Vector2::length(monster->pos - player->pos) < 48;
		});
	});
	REMOVE_FROM_LIST(projectiles, [&](std::unique_ptr<Projectile>& projectile){
		return projectile->state == ProjectileState::DeleteMe;
	});

	//monsterSpawner->logic(this, time);
	skybox->logic(time);
}

void World::render(){
	map->render(skybox->getLightColor());

	for (auto& player : players)
		player->render();
	for (auto& monster : monsters)
		monster->render();
	for (auto& projectile : projectiles)
		projectile->render();
}

bool World::areaHasBlocks(Vector2i px1, Vector2i px2, BlockCollisionType colType){
	return map->areaHasBlocks(px1,px2,
		[&](const BlockType& block){
			return (block.collisionType == colType);
		});
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
