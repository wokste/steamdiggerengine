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

int BuildEffect::run(EffectParams& params){
	Map& map = *params.entity.world->map;
	const Vector2i pos = Vector2::floorVec(params.targetPos);
	const int targetLayer = params.targetLayer;

	MapNode* node = map.getMapNode(pos.x, pos.y);

	if (!node || node->isset(targetLayer)){
		return 0;
	}

	if (targetLayer == Layer::front){
		if (params.entity.world->creatures().areaHasCreature(pos, pos + Vector2i(1,1)))
			return 0;
	}

	if (map.blocksAdjacent(pos.x, pos.y, targetLayer,
			[](const BlockType& block){return (block.collisionType == BlockCollisionType::Solid);}) == 0)
		return 0;

	node->setBlock(blockTypeID, targetLayer);
	LightingEngine::recalcAreaAround(map, pos);
	return 1;
}
