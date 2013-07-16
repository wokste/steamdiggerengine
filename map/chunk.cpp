#include "chunk.h"
#include "mapgenerator.h"

void Chunk::render(const Map& map, const sf::Color& skyColor, Texture& tileSet, const Vector2i& topLeft) const{
	for(int y = 0; y < height; ++y){
		for(int x = 0; x < width; ++x){
			nodes[x][y].render(map, skyColor, tileSet, Vector2i(x,y) + topLeft);
		}
	}
}

Chunk::Chunk(const Map& map, const MapGenerator& generator, const Vector2i& topLeft){
	// Fill all blocks given by the generator.
	for(int y = 0; y < height; ++y){
		for(int x = 0; x < width; ++x){
			for (int layer = 0; layer <= 1; ++layer){
				nodes[x][y].setBlock(map, generator.getBlock(x + topLeft.x,y + topLeft.y,layer), layer);
			}
		}
	}
}
