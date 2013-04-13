#include "mapgenerator.h"
#include "perlinnoise.h"
#include <cmath>

MapGenerator::MapGenerator(int seed){
	caveNoise = new PerlinNoise(seed + 1, 5, 0.5, 0.3);
	groundNoise = new PerlinNoise(seed + 2, 5, 0.5);
	typeNoise = new PerlinNoise(seed + 3, 2, 0.5);
}

MapGenerator::~MapGenerator(){
	delete caveNoise;
	delete groundNoise;
	delete typeNoise;
}

int MapGenerator::getBlockId(int x, int y, int layer){
	double caveVal = caveNoise->noiseSum(x, y);
	double groundVal = groundNoise->noiseSum(x, y) + 0.3 * (y - 15);
	double typeVal = typeNoise->noiseSum(x, y);
	if (groundVal < 0) // The sky
		return 0;

	if (layer == 0 && std::abs(caveVal) < 0.4) // The caves
		return 0;

	return (typeVal > 0.0) ? 1 : 2; // The stone and dirt
}
