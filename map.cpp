#include "map.h"
#include "texture.h"
#include "screen.h"
#include "items/block.h"
#include "items/itemdefmanager.h"
#include <iostream>
#include <math.h>

Map::Map(ItemDefManager* newItemDefs) :
	tiles(nullptr),
	mapWidth(128),
	mapHeight(256),
	tileWidth(16),
	tileHeight(16),
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
	tileSet = new Texture("tileset.png", Vector2i(16, 16));

	tiles = new Tile[mapWidth * mapHeight];

	for(int y = 0; y < mapHeight; y++){
		for(int x = 0; x < mapWidth; x++){
			auto t = tile(x, y);

			if (y > 26 + sin(x / 7.0) * 5 + sin(x / 11.0) * 5)
				t->blockId = ((int)(y + sin(x / 5.0) * 2) / 4) % 2 + 1;
			else
				t->blockId = 0;
		}
	}

	for(int y = 0; y < mapHeight; y++){
		for(int x = 0; x < mapWidth; x++){
			findTileFrame(x, y);
		}
	}

	return true;
}

void Map::findTileFrame(int x, int y){
	if (x < 0 || x > mapWidth || y < 0 || y > mapHeight)
		return;

	int tId       = tileNum(x,y);
	int tBlockId  = tiles[tId].blockId;
	Block* tBlock = dynamic_cast<Block*>(itemDefs->getItemDef(tBlockId));

	if (tBlock != nullptr){
		bool left  = (x == 0)			 || (tiles[tId - 1].blockId != tBlockId);
		bool right = (x == mapWidth - 1) || (tiles[tId + 1].blockId != tBlockId);
		bool up	   = (y == 0)			 || (tiles[tId - mapWidth].blockId != tBlockId);
		bool down  = (y == mapHeight- 1) || (tiles[tId + mapWidth].blockId != tBlockId);

		tiles[tId].frame = left * 1 + right * 2 + up * 4 + down * 8 + tBlock->startFrame;
		if (tiles[tId].blockId == 0) tiles[tId].frame = -1;
	}
}

void Map::render(){
	if(tileSet == nullptr) return;

	tileSet->bind(0xFFFFFFFF);
	for(int y = 0; y < mapHeight; y++){
		for(int x = 0; x < mapWidth; x++){
			Tile* t = &tiles[tileNum(x, y)];
			if (t->frame != -1){
				int tX  = (x * tileWidth);
				int tY  = (y * tileHeight);

				tileSet->drawTile(Vector2i(tX, tY), Vector2i(tileWidth, tileHeight), t->frame);
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
	return (y * mapWidth + x);
}


/// Gives the tiledata of a given tile.
inline Map::Tile* Map::tile(int x, int y){
	return &tiles[(y * mapWidth + x)];
}

void Map::unloadResources(){
	if (tiles	!= nullptr) delete[] tiles;
	if (tileSet  != nullptr) delete tileSet;
}

bool Map::validPos(int x1Px, int x2Px, int y1Px, int y2Px){
	int x1 = (int)x1Px / tileWidth;
	int x2 = (int)x2Px / tileWidth + 1;
	int y1 = (int)y1Px / tileHeight;
	int y2 = (int)y2Px / tileHeight + 1;

	if (x1 < 0 || x2 >= mapWidth || y2 >= mapHeight)
		return false;
	if (y1 < 0){
		y1 = 0;
	}

	for(int y = y1; y < y2; y++){
		for(int x = x1; x < x2; x++){
			Block* block = dynamic_cast<Block*>(itemDefs->getItemDef(tiles[tileNum(x,y)].blockId));
			if (block->collisionType == BlockCollisionType::Solid){
				return false;
			}
		}
	}
	return true;
}
