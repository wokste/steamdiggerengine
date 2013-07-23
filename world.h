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

	std::vector<Entity*> entities;
	// A subset of entities containing all creatures
	std::vector<Creature*> creatures;
	// A subset of entities containing all entities that will be deleted. This is added because removing from entities or creatures at the wrong moment invalidates an iterator.
	std::vector<Entity*> toDelete;
	std::unique_ptr<Skybox> skybox;
	std::unique_ptr<MonsterSpawner> monsterSpawner;

	void removeEntity(Entity* entity);
private:
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
};
