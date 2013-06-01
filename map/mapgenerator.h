#pragma once
#include <memory>
class PerlinNoise;
class BlockType;
class Map;

class MapGenerator{
public:
	MapGenerator(int seed, Map& map);
	MapGenerator(const MapGenerator&) = delete;
	~MapGenerator();
	int getBlock(int x, int y, int layer) const;
private:
	std::unique_ptr<PerlinNoise> caveNoise;
	std::unique_ptr<PerlinNoise> groundNoise;
	std::unique_ptr<PerlinNoise> typeNoise;
	Map& map;
};
