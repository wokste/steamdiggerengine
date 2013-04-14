#pragma once
#include <string>
#include "utils/vector2.h"
#include "items/enums.h"

class Texture;
class Screen;
class ItemDefManager;
class Block;
class MapGenerator;

class Map{
public:
	class Tile{
	public:
		int frame;
		int blockId;

		void setBlock(Block* block);
	};

	Vector2i  mapSize;
	Vector2i  tileSize;
	double    gravity = 25;

public:
	Map(int seed, ItemDefManager* newItemDefs);
	~Map();

	ItemDefManager* itemDefs;
	void generate();
	void render();
	bool areaHasBlocks(Vector2i px1, Vector2i px2, BlockCollisionType colType);
	Tile* tileRef(int x, int y, int layer);

private:
	Tile* tile(int x, int y, int layer);
	Texture* tileSet;
	Tile* tiles;
	MapGenerator* generator;

	int32_t seed;

	void unloadResources();
};
