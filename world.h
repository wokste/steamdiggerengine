#pragma once
#include <string>
#include <vector>
#include <memory>
#include "utils/vector2.h"
#include "items/enums.h"

class Map;
class Entity;
//class EntityList;
//class EntityStats;
class PlayerStats;
class MonsterStats;
class FlyingMonsterStats;
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

	void logic(int timeMs);
	void render();
	Player* spawn(PlayerStats* stats, Vector2d spawnPos);
	Projectile* spawn(ProjectileStats* stats, Vector2d spawnPos);
	Monster* spawn(FlyingMonsterStats* stats, Vector2d spawnPos);
	//bool validPos(int x1Px, int x2Px, int y1Px, int y2Px);
	bool areaHasBlocks(Vector2i px1, Vector2i px2, BlockCollisionType colType = BlockCollisionType::Solid);
	bool areaHasEntity(Vector2i px1, Vector2i px2);
	Game* game;

	std::vector<std::unique_ptr<Player>> players;
	std::vector<std::unique_ptr<Monster>> monsters;
	std::vector<std::unique_ptr<Projectile>> projectiles;
};
