#include "map.h"
#include "../utils/texture.h"
#include "../screen.h"
#include "../items/block.h"
#include "../items/itemdefmanager.h"
#include "../game.h"
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <SFML/Graphics/Color.hpp>

#include "mapgenerator.h"
#include "mapnode.h"
#include "lightingengine.h"
#include "../enums.h"

Map::Map(int seed, Game& game) :
	nodes(nullptr),
	mapSize(Vector2i(64,64)),
	tileSize(32,32),
	itemDefs(*game.itemDefs.get())
{
	tileSet.reset(new Texture(game.fileSystem.fullpath("tileset.png"), tileSize));
	generator.reset(new MapGenerator(seed, itemDefs));
}

Map::~Map(){
	if (nodes != nullptr) delete[] nodes;
}

void Map::generate(){
	if (nodes != nullptr)
		delete[] nodes;
	nodes = new MapNode[mapSize.x * mapSize.y];

	for(int y = 0; y < mapSize.y; y++){
		for(int x = 0; x < mapSize.x; x++){
			for (int layer = 0; layer <= 1; layer++){
				getMapNode(x, y)->setBlock(generator->getBlock(x,y,layer), layer);
			}
		}
	}
	LightingEngine::recalcArea(*this,Vector2i(0,0), mapSize);
}

void Map::logic(double time){
}

void Map::render(const sf::Color& skyColor) const{
	if(tileSet == nullptr) return;

	tileSet->bind();
	for(int y = 0; y < mapSize.y; y++){
		for(int x = 0; x < mapSize.x; x++){
			MapNode* node = getMapNode(x, y);
			node->render(skyColor, *tileSet, Vector2i(x,y));
		}
	}
}

MapNode* Map::getMapNode(int x, int y) const{
	if (x < 0 || x >= mapSize.x || y < 0 || y >= mapSize.y)
		return nullptr;

	return &nodes[(y * mapSize.x + x)];
}

sf::Color Map::getColor(const sf::Color& skyColor, Vector2d pos) const{
	MapNode* node = getMapNode((int)pos.x, (int)pos.y);
	return node ? (node->getLight(skyColor)) : skyColor;
}

bool Map::blockAdjacent(int x, int y, int layer, std::function<bool(Block*)> pred){
	// Test all adjacent tiles
	for (int i = 0 ; i < 4; i++){
		MapNode* node = getMapNode(x + (i == 0) - (i == 1),y + (i == 2) - (i == 3));
		if (node == nullptr){
			if (pred(nullptr))
				return true;
			continue;
		}
		Block* block = node->getBlock(itemDefs,layer);

		if (pred(block)){
			return true;
		}
	}
	return false;
}

bool Map::areaHasBlocks(Vector2i px1, Vector2i px2, std::function<bool(Block*)> pred){
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
			MapNode* node = getMapNode(x,y);
			if (node == nullptr){
				continue;
			}
			Block* block = node->getBlock(itemDefs, Layer::front);
			if (pred(block)){
				return true;
			}
		}
	}
	return false;
}
