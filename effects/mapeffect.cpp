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
#include "../utils/confignode.h"
#include "../screen.h"
#include "../game.h"
#include "../map/blocktype.h"

#include "../map/map.h"
#include "../map/mapnode.h"
#include "../map/lightingengine.h"
#include "../enums.h"

MineEffect::MineEffect(const ConfigNode& config){
	damageHigh = config.getInt("damage-high", 10);
	damageLow = config.getInt("damage-low", (damageHigh + 1) / 2);
	materialType = 255; //TODO
}

bool MineEffect::run(Entity& owner, Vector2d sourcePos, Vector2d targetPos, int targetLayer){
	Map& map = *owner.world->map;
	const Vector2i pos = Vector2::dToI(targetPos);
	MapNode* node = map.getMapNode(pos.x, pos.y);

	if (!node || !node->isset(targetLayer)){
		return false;
	}

	if (!map.blockAdjacent(pos.x, pos.y, targetLayer,
			[](const BlockType& block){return (block.collisionType == BlockCollisionType::Air);}))
		return false;

	const BlockType& minedBlock = node->getBlock(targetLayer);
	if (node->damageBlock(targetLayer, damageHigh, damageLow, materialType)){
		LightingEngine::recalcAreaAround(map, pos);
		auto dropPos = Vector2::iToD(pos) + Vector2d(0.5,0.5);
		minedBlock.drops.dropStuff(*owner.world, dropPos);
	}
	return true;
}

bool BuildEffect::run(Entity& owner, Vector2d sourcePos, Vector2d targetPos, int targetLayer){
	Map& map = *owner.world->map;
	const Vector2i pos = Vector2::dToI(targetPos);
	MapNode* node = map.getMapNode(pos.x, pos.y);

	if (!node || node->isset(targetLayer)){
		return false;
	}

	if (targetLayer == Layer::front){
		if (owner.world->areaHasEntity(pos, pos + Vector2i(1,1)))
			return false;
	}

	if (!map.blockAdjacent(pos.x, pos.y, targetLayer,
			[](const BlockType& block){return (block.collisionType == BlockCollisionType::Solid);}))
		return false;

	node->setBlock(blockTypeID, targetLayer);
	LightingEngine::recalcAreaAround(map, pos);
	return true;
}
