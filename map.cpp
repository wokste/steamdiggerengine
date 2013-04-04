#include "map.h"
#include "texture.h"
#include "screen.h"
#include "items/block.h"
#include "items/itemdefmanager.h"
#include <iostream>
#include <math.h>

Map::Map(ItemDefManager* newItemDefs) :
	tiles(nullptr),
	mapSize(Vector2i(128,64)),
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

	tiles = new Tile[mapSize.x * mapSize.y];

	for(int y = 0; y < mapSize.y; y++){
		for(int x = 0; x < mapSize.x; x++){
			auto t = tile(x, y);

			if (y > 26 + sin(x / 7.0) * 5 + sin(x / 11.0) * 5)
				t->blockId = ((int)(y + sin(x / 5.0) * 2) / 4) % 2 + 1;
			else
				t->blockId = 0;
		}
	}

	for(int y = 0; y < mapSize.y; y++){
		for(int x = 0; x < mapSize.x; x++){
			findTileFrame(x, y);
		}
	}

	return true;
}

void Map::findTileFrame(int x, int y){
	if (x < 0 || x > mapSize.x || y < 0 || y > mapSize.y)
		return;

	int tId       = tileNum(x,y);
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
			Tile* t = &tiles[tileNum(x, y)];
			if (t->frame != -1){
				Vector3i pos(x, y, 0);
				tileSet->drawBlock(pos, t->frame, 255);
			}
		}
	}
}

void Map::setTile(int x, int y, int blockId){
	auto tMid = tile(x,y);
	tMid->blockId = blockId;

	findTileFrame(x,   y);
	findTileFrame(x-1, y);
	findTileFrame(x+1, y);
	findTileFrame(x,   y-1);
	findTileFrame(x,   y+1);
}

/// Gives the tilenum of a given tile.
inline int Map::tileNum(int x, int y){
	return (y * mapSize.x + x);
}


/// Gives the tiledata of a given tile.
inline Map::Tile* Map::tile(int x, int y){
	return &tiles[(y * mapSize.x + x)];
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
			Block* block = dynamic_cast<Block*>(itemDefs->getItemDef(tiles[tileNum(x,y)].blockId));
			if (block->collisionType == colType){
				return true;
			}
		}
	}
	return false;
}
