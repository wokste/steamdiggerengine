#pragma once
#include <string>
#include "../utils/vector2.h"
#include "../items/enums.h"

class Texture;
class Screen;
class ItemDefManager;
class Game;
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
	Map(int seed, Game* settings);
	Map(const Map&) = delete;
	~Map();

	ItemDefManager* itemDefs;
	void generate();
	void render() const;
	bool areaHasBlocks(Vector2i px1, Vector2i px2, BlockCollisionType colType) const;
	bool blockAdjacent(int x, int y, int layer, BlockCollisionType colType) const;
	Tile* tileRef(int x, int y, int layer) const;
	Block* blockRef(int x, int y, int layer) const;

private:
	Tile* tile(int x, int y, int layer) const;
	Texture* tileSet;
	Tile* tiles;
	MapGenerator* generator;

	int32_t seed;

	void unloadResources();
};
