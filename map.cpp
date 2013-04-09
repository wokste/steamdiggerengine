#include "map.h"
#include "texture.h"
#include "screen.h"
#include "items/block.h"
#include "items/itemdefmanager.h"
#include <iostream>
#include <math.h>

constexpr int MAX_LAYERS = 2;

Map::Map(ItemDefManager* newItemDefs) :
	tiles(nullptr),
	mapSize(Vector2i(64,64)),
	tileSize(16,16),
	tileSet(nullptr),
	itemDefs(newItemDefs)
{
}

Map::~Map(){
	unloadResources();
}

bool Map::generate(int newSeed){
	seed = newSeed;
	//MapGenerator generator(seed);

	unloadResources();
	tileSet = new Texture("tileset.png", tileSize);

	tiles = new Tile[mapSize.x * mapSize.y * MAX_LAYERS];

	for(int y = 0; y < mapSize.y; y++){
		for(int x = 0; x < mapSize.x; x++){
			auto t = tile(x, y, 0);

			if (y > 26 + sin(x / 7.0) * 5 + sin(x / 11.0) * 5)
				t->blockId = ((int)(y + sin(x / 5.0) * 2) / 4) % 2 + 1;
			else
				t->blockId = 0;

			t = tile(x, y, 1);

			if (y > 26 + sin(x / 7.0) * 5 + sin(x / 11.0) * 5)
				t->blockId = ((int)(y + sin(x / 5.0) * 2) / 4) % 2 + 1;
			else
				t->blockId = 0;
		}
	}

	for(int y = 0; y < mapSize.y; y++){
		for(int x = 0; x < mapSize.x; x++){
			findTileFrame(x, y, 0);
			findTileFrame(x, y, 1);
		}
	}

	return true;
}

void Map::findTileFrame(int x, int y, int layer){
	if (x < 0 || x > mapSize.x || y < 0 || y > mapSize.y || layer < 0 || layer >= MAX_LAYERS)
		return;

	int tId       = tileNum(x,y, layer);
	int tBlockId  = tiles[tId].blockId;
	Block* tBlock = dynamic_cast<Block*>(itemDefs->getItemDef(tBlockId));

	if (tBlock != nullptr){
		tiles[tId].frame = tBlock->startFrame;
		if (tiles[tId].blockId == 0) tiles[tId].frame = -1;
	}
}

void Map::render(){
	if(tileSet == nullptr) return;

	tileSet->bind(0xFFFFFFFF);
	for(int y = 0; y < mapSize.y; y++){
		for(int x = 0; x < mapSize.x; x++){
			Tile* frontTile = &tiles[tileNum(x, y, 0)];
			if (frontTile->frame != -1){
				Vector3i pos(x, y, 0);
				tileSet->drawBlock(pos, frontTile->frame, 255);
			}

			Tile* backTile = &tiles[tileNum(x, y, 1)];
			if (backTile->frame != -1){
				Vector3i pos(x, y, 1);
				tileSet->drawBlock(pos, backTile->frame, 128);
			}
		}
	}
}

void Map::setTile(int x, int y, int layer, int blockId){
	auto tMid = tile(x, y, layer);
	tMid->blockId = blockId;

	findTileFrame(x, y, layer);
}

/// Gives the tilenum of a given tile.
inline int Map::tileNum(int x, int y, int layer){
	return (y * mapSize.x + x) * MAX_LAYERS + layer;
}


/// Gives the tiledata of a given tile.
inline Map::Tile* Map::tile(int x, int y, int layer){
	return &tiles[(y * mapSize.x + x) * MAX_LAYERS + layer];
}

void Map::unloadResources(){
	if (tiles	!= nullptr) delete[] tiles;
	if (tileSet  != nullptr) delete tileSet;
}

bool Map::areaHasBlocks(Vector2i px1, Vector2i px2, BlockCollisionType colType){
	int x1 = (int)(px1.x);
	int x2 = (int)(px2.x) + 1;
	int y1 = (int)(px1.y);
	int y2 = (int)(px2.y) + 1;

	if (x1 < 0 || x2 >= mapSize.x || y2 >= mapSize.y)
		return true;
	if (y1 < 0){
		y1 = 0;
	}

	for(int y = y1; y < y2; y++){
		for(int x = x1; x < x2; x++){
			Block* block = dynamic_cast<Block*>(itemDefs->getItemDef(tiles[tileNum(x,y,0)].blockId));
			if (block->collisionType == colType){
				return true;
			}
		}
	}
	return false;
}
