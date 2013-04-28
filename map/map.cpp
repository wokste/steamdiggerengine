#include "map.h"
#include "../utils/texture.h"
#include "../screen.h"
#include "../items/block.h"
#include "../items/itemdefmanager.h"
#include "../utils/gamesettings.h"
#include <iostream>
#include <cmath>
#include <stdlib.h>

#include "mapgenerator.h"

constexpr int MAX_LAYERS = 2;

void Map::Tile::setBlock(Block* block){
	if (block == nullptr){
		frame = -1;
		blockId = -1;
	} else {
		frame = block->startFrame + rand() % block->numFrames;
		blockId = block->ID;
	}
}

Map::Map(int seed, GameSettings* gameSettings) :
	tiles(nullptr),
	mapSize(Vector2i(64,64)),
	tileSize(32,32),
	itemDefs(gameSettings->itemDefs)
{
	tileSet = new Texture(gameSettings->findResource("tileset.png"), tileSize);
	generator = new MapGenerator(seed, itemDefs);
}

Map::~Map(){
	if (tiles != nullptr) delete[] tiles;
	delete tileSet;
	delete generator;
}

void Map::generate(){
	if (tiles != nullptr)
		delete[] tiles;
	tiles = new Tile[mapSize.x * mapSize.y * MAX_LAYERS];

	for(int y = 0; y < mapSize.y; y++){
		for(int x = 0; x < mapSize.x; x++){
			for (int layer = 0; layer <= 1; layer++){
				tile(x, y, layer)->setBlock(generator->getBlock(x,y,layer));
			}
		}
	}
}

void Map::render(){
	if(tileSet == nullptr) return;

	tileSet->bind(0xFFFFFFFF);
	for(int y = 0; y < mapSize.y; y++){
		for(int x = 0; x < mapSize.x; x++){
			Tile* frontTile = tile(x, y, 0);
			if (frontTile->frame != -1){
				Vector3i pos(x, y, 0);
				tileSet->drawBlock(pos, frontTile->frame, 255);
			}

			Tile* backTile = tile(x, y, 1);
			if (backTile->frame != -1){
				Vector3i pos(x, y, 1);
				tileSet->drawBlock(pos, backTile->frame, 128);
			}
		}
	}
}


/// Gives the tiledata of a given tile.
Map::Tile* Map::tile(int x, int y, int layer){
	return &tiles[(y * mapSize.x + x) * MAX_LAYERS + layer];
}

/// Gives the tiledata of a given tileor nullptr if it is not a valid tile.
Map::Tile* Map::tileRef(int x, int y, int layer){
	if (x < 0 || x >= mapSize.x || y < 0 || y >= mapSize.y)
		return nullptr;

	return &tiles[(y * mapSize.x + x) * MAX_LAYERS + layer];
}

bool Map::blockAdjacent(int x, int y, int layer, BlockCollisionType colType){
	// Test all adjacent tiles
	for (int i = 0 ; i < 4; i++){
		Block* block = blockRef(x + (i == 0) - (i == 1),y + (i == 2) - (i == 3),layer);

		if (block == nullptr){
			if (colType == BlockCollisionType::Air)
				return true;

			continue;
		}

		if (block->collisionType == colType)
			return true;
	}
	return false;
}

Block* Map::blockRef(int x, int y, int layer){
	Tile* tile = tileRef(x,y,layer);
	if (tile == nullptr || tile->blockId < 0)
		return nullptr;
	return (*itemDefs)[tile->blockId]->asBlock();
}

bool Map::areaHasBlocks(Vector2i px1, Vector2i px2, BlockCollisionType colType){
	int x1 = (int)(px1.x);
	int x2 = (int)(px2.x) + 1;
	int y1 = (int)(px1.y);
	int y2 = (int)(px2.y) + 1;

	if (x1 < 0 || x2 > mapSize.x || y2 > mapSize.y)
		return true;
	if (y1 < 0){
		y1 = 0;
	}

	for(int y = y1; y < y2; y++){
		for(int x = x1; x < x2; x++){
			auto bId = tile(x,y,0)->blockId;
			if (bId >= 0){
				Block* block = (*itemDefs)[bId]->asBlock();
				if (block != nullptr && block->collisionType == colType){
					return true;
				}
				if (block == nullptr && colType == BlockCollisionType::Air){
					return true;
				}
			}
		}
	}
	return false;
}
