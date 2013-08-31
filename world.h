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
#pragma once
#include <string>
#include <vector>
#include <memory>
#include "utils/vector2.h"
#include "enums.h"
#include "utils/monsterspawner.h"

class Map;
class Entity;
class Creature;
class Skybox;
class Screen;

class World{
public:
	std::unique_ptr<Map> map;
	World();
	World(const World& that) = delete;
	~World();

	void logic(double time);
	void render(const Screen& screen);
	bool areaHasBlocks(Vector2i px1, Vector2i px2, BlockCollisionType colType = BlockCollisionType::Solid);
	bool areaHasEntity(Vector2i px1, Vector2i px2);
	bool damageBlock(Vector2i pos, int targetLayer, int damageHigh, int damageLow, int materialType);

	void removeEntity(Entity* entity);
	void addEntity(Entity* entity);
public:
	template <class T>
	T* spawn(T& prototype, Vector2d newPos){
		T* spawned = nullptr;
		if (prototype.validPos(*this, newPos)){
			spawned = new T(prototype);
			spawned->setPos(this, newPos);
			addEntity(spawned);
		}
		return spawned;
	}

	void forEachEntity(std::function<void(Entity&)>);
	void forEachCreature(std::function<void(Creature&)>);
private:
	std::vector<Entity*> entities;
	// A subset of entities containing all creatures
	std::vector<Creature*> creatures;
	// A subset of entities containing all entities that will be added/deleted. This is added because adding/removing from entities or creatures at the wrong moment invalidates an iterator.
	std::vector<Entity*> toDelete;
	std::vector<Entity*> toAdd;
	std::unique_ptr<Skybox> skybox;
	std::unique_ptr<MonsterSpawner> monsterSpawner;
};
