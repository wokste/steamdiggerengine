#pragma once
#include <string>
#include <vector>
#include <memory>
#include "utils/vector2.h"
#include "items/enums.h"
#include "utils/monsterspawner.h"

class Map;
class Entity;
class PlayerStats;
class MonsterStats;
class ProjectileStats;
class Player;
class Monster;
class Projectile;
class Game;

class World{

public:
	Map* map;
	World(Game* newSettings);
	World(const World& that) = delete;
	~World();

	void logic(double time);
	void render();
	bool areaHasBlocks(Vector2i px1, Vector2i px2, BlockCollisionType colType = BlockCollisionType::Solid);
	bool areaHasEntity(Vector2i px1, Vector2i px2);
	Game* game;

	std::vector<std::unique_ptr<Player>> players;
	std::vector<std::unique_ptr<Monster>> monsters;
	std::vector<std::unique_ptr<Projectile>> projectiles;

	MonsterSpawner monsterSpawner;
};
