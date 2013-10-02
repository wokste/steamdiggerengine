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
#include "attackeffect.h"
#include "../entities/creature.h"
#include "../world.h"
#include <pugixml.hpp>
#include "../utils/vector2.h"
#include "../utils/mathplus.h"
#include "../map/map.h"

AttackEffect::AttackEffect(pugi::xml_node& node){
	attack.load(node);
	radius = node.attribute("radius").as_float(0);
	reach = node.attribute("reach").as_float(0);
}

int AttackEffect::run(EffectParams& params){
	//Find position to target.
	bool shouldMine = attack.damageTerrain;
	Vector2d pos = params.targetPos;
	if (Vector2::length(params.sourcePos - pos) > reach){
		pos = Vector2::setLength(pos - params.sourcePos, reach) + params.sourcePos;
		shouldMine = false;
	}

	World& world = *params.entity.world;

	// Attack the nodes on the map
	if (shouldMine){
		for (int x = MathPlus::floorInt(pos.x - radius); x < MathPlus::ceilInt(pos.x + radius); x++){
			for (int y = MathPlus::floorInt(pos.y - radius); y < MathPlus::ceilInt(pos.y + radius); y++){
				world.map->damageBlock(Vector2i(x,y), params.targetLayer, attack, world);
			}
		}
	}

	// Attack the entities near me
	if (params.eventInstignator)
		for (auto other: world.creatures())
			if (params.eventInstignator->aggressiveTo(other) && (abs(other->pos.x - pos.x) < radius + other->collision.x) && (abs(other->pos.y - pos.y) < radius + other->collision.y))
				other->takeDamage(attack, params.sourcePos);

	return true;
}
