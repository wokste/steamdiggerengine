#include "map.h"
#include "../utils/texture.h"
#include "../utils/confignode.h"
#include "../screen.h"
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
#include "chunk.h"
#include "blocktype.h"

bool ChunkSorter::operator()(const Vector2i& a, const Vector2i& b) const {
	return a.x < b.x || (a.x == b.x && a.y < b.y);
}

Map::Map(int seed) :
	tileSize(32,32)
{
	tileSet.reset(new Texture(GameGlobals::fileSystem.fullpath("tileset.png"), tileSize));
	generator.reset(new MapGenerator(seed, *this));
}

Map::~Map(){
}

void Map::generateAround(const Vector2d& pos){
	Vector2i posMin((int)pos.x - 48, (int)pos.y - 48);
	Vector2i posMax((int)pos.x + 48, (int)pos.y + 48);
	for (int x = posMin.x & ~Chunk::widthMask ; x < posMax.x; x+=Chunk::width){
		for (int y = posMin.y & ~Chunk::heightMask; y < posMax.y; y+=Chunk::height){
			Vector2i chunkNum(x,y);
			if (chunks.count(chunkNum) >= 1)
				continue;

			chunks.insert(std::make_pair(chunkNum, new Chunk(*generator, chunkNum)));
			LightingEngine::recalcArea(*this, chunkNum, chunkNum + Vector2i(Chunk::width, Chunk::height));
		}
	}
}

void Map::logic(double time){
}

void Map::render(const Screen& screen, const sf::Color& skyColor) const{
	if(tileSet == nullptr) return;

	tileSet->bind();
	for (auto iter = chunks.begin(); iter != chunks.end(); iter++) {
		// iter->first : Vector2; iter->second : Chunk
		if (screen.containsArea(iter->first, Vector2i(Chunk::width, Chunk::height)))
			iter->second->render(skyColor, iter->first);
	}
}

MapNode* Map::getMapNode(int x, int y) const{
	Vector2i key = Vector2i(x & ~(Chunk::widthMask),y & ~(Chunk::heightMask));
	// TODO: This can be optimized. One lookup too much
	if (chunks.find(key) == chunks.end()) {
		return nullptr;
	} else {
		Map* mutableThis = const_cast<Map*>(this);
		Chunk* chunk = mutableThis->chunks[key];
		return &(chunk->nodes[x & Chunk::widthMask][y & Chunk::heightMask]);
	}
}

sf::Color Map::getColor(const sf::Color& skyColor, Vector2d pos) const{
	MapNode* node = getMapNode((int)pos.x, (int)pos.y);
	return node ? (node->getLight(skyColor)) : skyColor;
}

bool Map::blockAdjacent(int x, int y, int layer, std::function<bool(const BlockType&)> pred){
	// Test all adjacent tiles
	for (int i = 0 ; i < 4; i++){
		MapNode* node = getMapNode(x + (i == 0) - (i == 1),y + (i == 2) - (i == 3));
		if (node == nullptr){
			continue;
		}
		if (pred(node->getBlock(layer))){
			return true;
		}
	}
	return false;
}

bool Map::areaHasBlocks(Vector2i px1, Vector2i px2, std::function<bool(const BlockType&)> pred){
	int x1 = (int)(px1.x);
	int x2 = (int)(px2.x) + 1;
	int y1 = (int)(px1.y);
	int y2 = (int)(px2.y) + 1;

	for(int y = y1; y < y2; ++y){
		for(int x = x1; x < x2; ++x){
			MapNode* node = getMapNode(x,y);
			if (node == nullptr){
				continue;
			}

			if (pred(node->getBlock(Layer::front))){
				return true;
			}
		}
	}
	return false;
}
