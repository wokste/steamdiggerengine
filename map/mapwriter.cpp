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
#include "../world.h"
#include "mapwriter.h"
#include "lightingengine.h"
#include <limits.h>
#include "mapnode.h"
#include <iostream>
#include "../entities/creature.h"
#include "blocktype.h"

MapWriter::MapWriter(World& world) : world(world), area(){}

MapWriter::~MapWriter(){
	recalcLighting();
}

bool MapWriter::place(Vector2i pos, int layer, int blockTypeID){
	MapNode* node = world.map->getMapNode(pos.x, pos.y);
	if (!node || node->isset(layer)){
		return false;
	}

	if (layer == Layer::front)
		for (auto creature: world.creatures())
			if (creature->isInArea(Vector2::iToD(pos), Vector2::iToD(pos) + Vector2d(1,1)))
				return false;

	node->setBlock(blockTypeID, layer);
	pointChanged(pos);
	return true;
}

bool MapWriter::damage(Vector2i pos, int layer, const int damage, const int damageType){
	MapNode* node = world.map->getMapNode(pos.x, pos.y);
	if (!node || !node->isset(layer)){
		return false;
	}
	if (layer == Layer::back && pos.y > 20)
		return false;

	auto& block = node->getBlock(layer);
	if (node->damageBlock(layer, damage, damageType)){
		destroy(pos, layer, damageType);
	}
	return true;
}

void MapWriter::destroy(Vector2i pos, int layer, const int damageType, int recursionTTL, int blockFlags){
	MapNode* node = world.map->getMapNode(pos.x, pos.y);
	if (!node || !node->isset(layer))
		return;
	auto& block = node->getBlock(layer);
	if (blockFlags == 0 || (block.flags & blockFlags)){
		block.drops.dropStuff(world, Vector2::center(pos), damageType);
		node->setBlock(0, layer);
		pointChanged(pos);

		if (recursionTTL > 0){
			recursionTTL--;
			destroy(pos + Vector2i(-1,0), layer, damageType, recursionTTL, BlockType::FLAG_ATTACH_RIGHT);
			destroy(pos + Vector2i(1,0) , layer, damageType, recursionTTL, BlockType::FLAG_ATTACH_LEFT);
			destroy(pos + Vector2i(0,-1), layer, damageType, recursionTTL, BlockType::FLAG_ATTACH_BOTTOM);
			destroy(pos + Vector2i(0,1) , layer, damageType, recursionTTL, BlockType::FLAG_ATTACH_TOP);
			if (layer > 0)
				destroy(pos, layer - 1, damageType, recursionTTL, BlockType::FLAG_ATTACH_BACK);
			if (layer < Layer::back)
				destroy(pos, layer + 1, damageType, recursionTTL, BlockType::FLAG_ATTACH_FRONT);
		}
	}
}

bool MapWriter::solid(Vector2i topLeftPos, Vector2i size, int layer){
	if (layer == Layer::front)
		for (auto creature: world.creatures()){
			auto bottomRightPos = topLeftPos + size;
			if (creature->isInArea(Vector2::iToD(topLeftPos), Vector2::iToD(bottomRightPos)))
				return true;
		}

	for (int x = topLeftPos.x; x < size.x + topLeftPos.x; x++){
		for (int y = topLeftPos.y; y < size.y + topLeftPos.y; y++){
			MapNode* node = world.map->getMapNode(x, y);
			if (!node || node->isset(layer))
				return true;
		}
	}
	return false;
}

void MapWriter::recalcLighting(){
	if (area.width == 0 || area.height == 0)
		return; // No changes

	constexpr int lightRadius = 255 / 20 + 1;
	Vector2i pos1 = Vector2i(area.left - lightRadius, area.top - lightRadius);
	Vector2i pos2 = Vector2i(area.left + (area.width - 1) + lightRadius, area.top + (area.height - 1) + lightRadius);
	LightingEngine::recalcArea(*world.map, pos1, pos2);
	area = Rect4i();
}

void MapWriter::pointChanged(Vector2i pos){
	if (area.width == 0 || area.height == 0){
		area = Rect4i(pos, Vector2i(1,1));
		return;
	}
	if (pos.x < area.left){
		area.width += (pos.x - area.left);
		area.left = pos.x;
	}else if (pos.x > area.left + area.width){
		area.width = (pos.x - area.left);
	}

	if (pos.y < area.top){
		area.height += (pos.y - area.top);
		area.top = pos.x;
	}else if (pos.y > area.top + area.height){
		area.height = (pos.y - area.top);
	}
}
