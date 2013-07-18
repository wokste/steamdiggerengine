#pragma once
#include <string>
#include <vector>
#include <memory>
#include "utils/vector2.h"
#include "enums.h"
#include "utils/monsterspawner.h"

class Map;
class Entity;
class Player;
class Monster;
class Projectile;
class Skybox;

class World{
public:
	std::unique_ptr<Map> map;
	World();
	World(const World& that) = delete;
	~World();

	void logic(double time);
	void render();
	bool areaHasBlocks(Vector2i px1, Vector2i px2, BlockCollisionType colType = BlockCollisionType::Solid);
	bool areaHasEntity(Vector2i px1, Vector2i px2);

	std::vector<std::unique_ptr<Player>> players;
	std::vector<std::unique_ptr<Monster>> monsters;
	std::vector<std::unique_ptr<Projectile>> projectiles;
	std::unique_ptr<Skybox> skybox;
	std::unique_ptr<MonsterSpawner> monsterSpawner;

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
