#pragma once

class PerlinNoise;
class Block;
class ItemDefManager;

class MapGenerator{
public:
	MapGenerator(int seed, ItemDefManager* newItemDefs);
	~MapGenerator();
	Block* getBlock(int x, int y, int layer) const;
private:
	PerlinNoise* caveNoise;
	PerlinNoise* groundNoise;
	PerlinNoise* typeNoise;
	ItemDefManager* itemDefs;
};
