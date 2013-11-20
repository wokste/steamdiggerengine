/*
The MIT License (MIT)

Copyright (c) 2013, Steven Wokke

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "src/map/mapgenerator.h"
#include "src/map/perlinnoise.h"
#include "src/items/itemdefmanager.h"
#include "src/map/mapnode.h"
#include <cmath>
#include "src/enums.h"

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
