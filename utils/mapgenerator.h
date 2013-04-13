#pragma once

class PerlinNoise;

class MapGenerator{
public:
	MapGenerator(int seed);
	~MapGenerator();
	int getBlockId(int x, int y, int layer);
private:
	PerlinNoise* caveNoise;
	PerlinNoise* groundNoise;
	PerlinNoise* typeNoise;
};
