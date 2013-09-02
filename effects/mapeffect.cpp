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
#include "mapeffect.h"
#include "../entities/player.h"

#include "../world.h"
#include <pugixml.hpp>
#include "../screen.h"
#include "../game.h"
#include "../map/blocktype.h"

#include "../map/map.h"
#include "../map/mapnode.h"
#include "../map/lightingengine.h"
#include "../enums.h"

MineEffect::MineEffect(pugi::xml_node& node){
	mineAttack.load(node);
}

bool MineEffect::run(Entity& owner, Vector2d sourcePos, Vector2d targetPos, int targetLayer){
	const Vector2i pos = chooseBlockToMine(*owner.world->map, sourcePos, targetPos, targetLayer);

	owner.world->damageBlock(pos, targetLayer, mineAttack);
	return true;
}

Vector2i MineEffect::chooseBlockToMine(Map& map, Vector2d& sourcePos, Vector2d& targetPos, int& targetLayer){
	Vector2i pos = Vector2::floorVec(targetPos);
	MapNode* node = map.getMapNode(pos.x, pos.y);
	if (node && node->isset(Layer::front)){
		targetLayer = Layer::front;
	}
	return pos;
}

bool BuildEffect::run(Entity& owner, Vector2d sourcePos, Vector2d targetPos, int targetLayer){
	Map& map = *owner.world->map;
	const Vector2i pos = Vector2::floorVec(targetPos);
	MapNode* node = map.getMapNode(pos.x, pos.y);

	if (!node || node->isset(targetLayer)){
		return false;
	}

	if (targetLayer == Layer::front){
		if (owner.world->areaHasEntity(pos, pos + Vector2i(1,1)))
			return false;
	}

	if (map.blocksAdjacent(pos.x, pos.y, targetLayer,
			[](const BlockType& block){return (block.collisionType == BlockCollisionType::Solid);}) == 0)
		return false;

	node->setBlock(blockTypeID, targetLayer);
	LightingEngine::recalcAreaAround(map, pos);
	return true;
}
