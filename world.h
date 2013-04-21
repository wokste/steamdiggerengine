#pragma once
#include <string>
#include "utils/vector2.h"
#include "items/enums.h"

class Map;
class Entity;
class EntityList;
class EntityStats;

class World{

public:
	Map * map;
	EntityList * entities;
	World();
	World(const World& that) = delete;
	~World();

	void logic(int timeMs);
	void render();
	Entity* spawn(std::string type, Vector2d spawnPos);
	Entity* spawn(EntityStats* stats, Vector2d spawnPos);
	//bool validPos(int x1Px, int x2Px, int y1Px, int y2Px);
	bool areaHasBlocks(Vector2i px1, Vector2i px2, BlockCollisionType colType = BlockCollisionType::Solid);
	bool areaHasEntity(Vector2i px1, Vector2i px2);
};