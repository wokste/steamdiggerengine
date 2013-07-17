#include "chunk.h"
#include "mapgenerator.h"

void Chunk::render(const sf::Color& skyColor, const Vector2i& topLeft) const{
	for(int y = 0; y < height; ++y){
		for(int x = 0; x < width; ++x){
			nodes[x][y].render(skyColor, Vector2i(x,y) + topLeft);
		}
	}
}

Chunk::Chunk(const MapGenerator& generator, const Vector2i& topLeft){
	// Fill all blocks given by the generator.
	for(int y = 0; y < height; ++y){
		for(int x = 0; x < width; ++x){
			for (int layer = 0; layer <= 1; ++layer){
				nodes[x][y].setBlock(generator.getBlock(x + topLeft.x,y + topLeft.y,layer), layer);
			}
		}
	}
}
