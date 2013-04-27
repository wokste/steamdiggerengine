#include "mapgenerator.h"
#include "perlinnoise.h"
#include "../items/block.h"
#include "../items/itemdefmanager.h"
#include <cmath>

MapGenerator::MapGenerator(int seed, ItemDefManager* newItemDefs) :
	itemDefs(newItemDefs)
{
	caveNoise = new PerlinNoise(seed + 1, 5, 0.5, 0.5);
	groundNoise = new PerlinNoise(seed + 2, 5, 0.5, 0.6);
	typeNoise = new PerlinNoise(seed + 3, 2, 0.5);
}

MapGenerator::~MapGenerator(){
	delete caveNoise;
	delete groundNoise;
	delete typeNoise;
}

Block* MapGenerator::getBlock(int x, int y, int layer){
	double caveVal = caveNoise->noiseSum(x, y);
	double groundVal = groundNoise->noiseSum(x, y) + 0.4 * (y - 15);
	double typeVal = typeNoise->noiseSum(x, y);
	if (groundVal < 0) // The sky
		return nullptr;

	if (layer == 0 && std::abs(caveVal) < 0.3) // The caves
		return nullptr;

	return (*itemDefs)[(typeVal > 0.0) ? 0 : 1]->asBlock(); // The stone and dirt
}
