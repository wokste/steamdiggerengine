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
#include "attack.h"
#include <SFML/Graphics/Color.hpp>
#include <iostream>
#include <assert.h>
#include "utils/mathplus.h"

World::World()
{
	map.reset(new Map(rand()));
	entities.reset(new EntityList());
	monsterSpawner.reset(new MonsterSpawner());
	skybox.reset(new Skybox());
}

World::~World(){
}

void World::logic(double time){
	map->logic(time);
	entities->logic(time, *this);
	monsterSpawner->logic(this, time);
	skybox->logic(time);
}

void World::render(const Screen& screen){
	map->render(screen, skybox->getLightColor());
	entities->render(screen, *map, skybox->getLightColor());
}

bool World::areaOccupied(Vector2d pos1, Vector2d pos2){
	return map->areaHasBlocks(Vector2::floorVec(pos1),Vector2::ceilVec(pos2),
		[&](const BlockType& block){
			return (block.collisionType == BlockCollisionType::Solid);
		});
}
