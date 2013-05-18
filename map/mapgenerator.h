#pragma once
#include <memory>
class PerlinNoise;
class Block;
class ItemDefManager;

class MapGenerator{
public:
	MapGenerator(int seed, ItemDefManager* newItemDefs);
	MapGenerator(const MapGenerator&) = delete;
	~MapGenerator();
	Block* getBlock(int x, int y, int layer) const;
private:
	std::unique_ptr<PerlinNoise> caveNoise;
	std::unique_ptr<PerlinNoise> groundNoise;
	std::unique_ptr<PerlinNoise> typeNoise;
	ItemDefManager* itemDefs;
};
