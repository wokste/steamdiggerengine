/*
The MIT License (MIT)

Copyright (c) 2013, Steven Wokke

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "world.h"
#include "entities/entity.h"
#include "map/map.h"
#include "map/mapnode.h"
#include "map/lightingengine.h"
#include "items/itemdefmanager.h"
#include "game.h"
#include <stdlib.h>
#include <pugixml.hpp>
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

	// == Addition of entities ==
	for (auto adding: toAdd){
		auto creature = dynamic_cast<Creature*>(adding);

		if (creature != nullptr){
			creatures.push_back(creature);
		}
		entities.push_back(adding);
	}
	toAdd.clear();

	// == Final logic ==
	monsterSpawner->logic(this, time);
	skybox->logic(time);
}

void World::render(const Screen& screen){
	map->render(screen, skybox->getLightColor());

	for (auto& entity : entities){
		sf::Color color = map->getColor(skybox->getLightColor(), entity->pos);
		entity->render(color);
	}
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
	toAdd.push_back(entity);
}

void World::removeEntity(Entity* entity){
	if (std::find(toDelete.begin(), toDelete.end(), entity)==toDelete.end())
		toDelete.push_back(entity);
}

void World::forEachEntity(std::function<void(Entity&)> func){
	for(auto& e: entities)
		func(*e);
}

void World::forEachCreature(std::function<void(Creature&)> func){
	for(auto& e: creatures)
		if (e->alive())
			func(*e);
}

bool World::damageBlock(Vector2i pos, int targetLayer, int damageHigh, int damageLow, int materialType){
	MapNode* node = map->getMapNode(pos.x, pos.y);
	if (!node || !node->isset(targetLayer))
		return false;

	const BlockType& minedBlock = node->getBlock(targetLayer);
	if (targetLayer == Layer::back && !minedBlock.artificial && (map->blocksAdjacent(pos.x, pos.y, targetLayer,
			[](const BlockType& block){return (block.collisionType == BlockCollisionType::Air);}) < 2))
		return false;

	if (node->damageBlock(targetLayer, damageHigh, damageLow, materialType)){
		LightingEngine::recalcAreaAround(*map, pos);
		auto dropPos = Vector2::iToD(pos) + Vector2d(0.5,0.5);
		minedBlock.drops.dropStuff(*this, dropPos);
	}
}
