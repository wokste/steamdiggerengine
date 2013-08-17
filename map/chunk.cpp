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
#include "chunk.h"
#include "mapgenerator.h"

void Chunk::render(const sf::Color& skyColor, const Vector2i& topLeft, int focussedLayer) const{
	for(int y = 0; y < height; ++y){
		for(int x = 0; x < width; ++x){
			nodes[x][y].render(skyColor, Vector2i(x,y) + topLeft, focussedLayer);
		}
	}
}

Chunk::Chunk(const MapGenerator& generator, const Vector2i& topLeft){
	// Fill all blocks given by the generator.
	for(int y = 0; y < height; ++y){
		for(int x = 0; x < width; ++x){
			for (int layer = 0; layer < Layer::count; ++layer){
				nodes[x][y].setBlock(generator.getBlock(x + topLeft.x,y + topLeft.y,layer), layer);
			}
		}
	}
}
