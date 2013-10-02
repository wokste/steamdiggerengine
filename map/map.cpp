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
#include "map.h"
#include "../utils/texture.h"
#include <pugixml.hpp>
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

Map::Map(int seed)
{
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
	if(GameGlobals::tileSet == nullptr) return;

	GameGlobals::tileSet->bind();
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY );

	bool selectedLayer  = screen.getSelectedLayer();
	for (auto iter = chunks.begin(); iter != chunks.end(); iter++) {
		auto& topLeftCorner = iter->first;
		auto& chunk         = iter->second;
		if (screen.containsArea(topLeftCorner, Vector2i(Chunk::width, Chunk::height)))
			chunk->render(skyColor, topLeftCorner, selectedLayer);
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY );
}

MapNode* Map::getMapNode(int x, int y) const{
	Vector2i key = Vector2i(x & ~(Chunk::widthMask),y & ~(Chunk::heightMask));
	auto iterator = chunks.find(key);
	if (iterator == chunks.end()) {
		return nullptr;
	} else {
		return &(iterator->second->nodes[x & Chunk::widthMask][y & Chunk::heightMask]);
	}
}

sf::Color Map::getColor(const sf::Color& skyColor, Vector2d pos) const{
	MapNode* node = getMapNode((int)pos.x, (int)pos.y);
	return node ? (node->getLight(skyColor)) : skyColor;
}

int Map::blocksAdjacent(int x, int y, int layer, std::function<bool(const BlockType&)> pred){
	int count = 0;
	// Test all adjacent tiles
	for (int i = 0 ; i < 4; i++){
		MapNode* node = getMapNode(x + (i == 0) - (i == 1),y + (i == 2) - (i == 3));
		if (node == nullptr){
			continue;
		}
		if (pred(node->getBlock(layer))){
			count++;
		}
	}
	return count;
}

bool Map::areaHasBlocks(Vector2i px1, Vector2i px2, std::function<bool(const BlockType&)> pred){
	int x1 = (int)(px1.x);
	int x2 = (int)(px2.x);
	int y1 = (int)(px1.y);
	int y2 = (int)(px2.y);

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

void Map::damageBlock(Vector2i pos, int targetLayer, const Attack& attack, World& world){
	MapNode* node = getMapNode(pos.x, pos.y);
	if (!node || !node->isset(targetLayer))
		return;

	if (node->isset(Layer::front))
		targetLayer = Layer::front;

	const BlockType& minedBlock = node->getBlock(targetLayer);
	if (targetLayer == Layer::back && !minedBlock.artificial && (blocksAdjacent(pos.x, pos.y, targetLayer,
			[](const BlockType& block){return (block.collisionType == BlockCollisionType::Air);}) < 2))
		return;

	if (node->damageBlock(targetLayer, attack))
	{ // Block is destroyed
		LightingEngine::recalcAreaAround(*this, pos);
		minedBlock.drops.dropStuff(world, Vector2::center(pos));
	}
}

bool Map::solid(int x, int y){
	auto node = getMapNode(x, y);
	return (node && node->getBlock(Layer::front).collisionType == BlockCollisionType::Solid);
}
