#include "mapgenerator.h"
#include "perlinnoise.h"
#include "../items/itemdefmanager.h"
#include "mapnode.h"
#include <cmath>
#include "../enums.h"

MapGenerator::MapGenerator(int seed, Map& newMap) : map(newMap)
{
	caveNoise.reset(new PerlinNoise(seed + 1, 5, 0.5, 30));
	groundNoise.reset(new PerlinNoise(seed + 2, 5, 0.5, 10));
	typeNoise.reset(new PerlinNoise(seed + 3, 5, 0.5,5));
}

MapGenerator::~MapGenerator(){
}

int MapGenerator::getBlock(int x, int y, int layer) const{
	double caveVal = caveNoise->noise2d(x, y);
	double groundVal = groundNoise->noise2d(x, y) + 0.1 * (y - 15);
	double typeVal = typeNoise->noise2d(x, y);
	if (groundVal < 0) // The sky
		return 0;

	if (layer == Layer::front && std::abs(caveVal) < 0.05) // The caves
		return 0;

	return (typeVal > 0.0) ? 1 : 2; // The stone and dirt
}
