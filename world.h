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
#include "entities/entitylist.h"

class Attack;
class Map;
class EntityList;
class Entity;
class Creature;
class Skybox;
class Screen;

class World{
public:
	std::unique_ptr<Map> map;
	std::unique_ptr<EntityList> entities;
	World();
	World(const World& that) = delete;
	~World();

	void logic(double time);
	void render(const Screen& screen);
	EntityListCreatureView creatures() {return entities->getCreatures();};
	bool damageArea(Vector2d pos, double radius, int targetLayer, const Attack& attack);
	bool areaOccupied(Vector2d pos1, Vector2d pos2);

	template <class T>
	T* spawn(T& prototype, Vector2d newPos){
		T* spawned = nullptr;
		if (prototype.validPos(*this, newPos)){
			spawned = new T(prototype);
			spawned->setPos(this, newPos);
			entities->add(spawned);
		}
		return spawned;
	}


private:
	std::unique_ptr<Skybox> skybox;
	std::unique_ptr<MonsterSpawner> monsterSpawner;
};
