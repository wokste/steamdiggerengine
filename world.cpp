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
#include <assert.h>

World::World()
{
	map.reset(new Map(rand()));
	monsterSpawner.reset(new MonsterSpawner());
	skybox.reset(new Skybox());
}

World::~World(){
	for (auto entity: entities){
		delete entity;
	}
}

void World::logic(double time){
	map->logic(time);

	for (auto entity: entities){
		entity->logic(time);

		if (entity->isPlayer)
			map->generateAround(entity->pos);
	}

	// == Removal of entities ==
	for (auto removing: toDelete){
		entities.erase(std::remove(entities.begin(), entities.end(), removing),entities.end());

		auto removingCreature = dynamic_cast<Creature*>(removing);
		if (removingCreature != nullptr){
			creatures.erase(std::remove(creatures.begin(), creatures.end(), removingCreature),creatures.end());
			for (auto entity: entities)
				entity->onCreatureDied(removingCreature);
		}

		delete removing;
	}
	toDelete.clear();

	// == Final logic ==
	monsterSpawner->logic(this, time);
	skybox->logic(time);
}

void World::render(const Screen& screen){
	map->render(screen, skybox->getLightColor());

	for (auto& entity : entities)
		entity->render();
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
	for (auto creature : creatures)
		if (creature->isInArea(px1d, px2d)) return true;
	return false;
}

void World::addEntity(Entity* entity){
	auto creature = dynamic_cast<Creature*>(entity);

	if (creature != nullptr){
		creatures.push_back(creature);
	}
	entities.push_back(entity);
}

void World::removeEntity(Entity* entity){
	if (std::find(toDelete.begin(), toDelete.end(), entity)==toDelete.end())
		toDelete.push_back(entity);
}
