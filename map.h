#pragma once
#include <string>
#include "utils/vector2.h"
#include "items/enums.h"

class Texture;
class Screen;
class ItemDefManager;

class Map{
public:
	class Tile{
	public:
		int frame;
		int blockId;
	};

	Vector2i  mapSize;
	Vector2i  tileSize;
	double    gravity = 400;

public:
	Map(ItemDefManager* newItemDefs);
	~Map();

	ItemDefManager* itemDefs;
	bool generate(int seed);
	void render();
	void setTile(int x, int y, int blockId);
	bool areaHasBlocks(Vector2i px1, Vector2i px2, BlockCollisionType colType);
	Tile* tile(int x, int y);
private:
	Texture* tileSet;
	Tile* tiles;

	int tileNum(int x, int y);
	int32_t seed;

	void unloadResources();
	void findTileFrame(int x, int y);
};
