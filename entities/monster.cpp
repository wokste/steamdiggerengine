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
#include "monster.h"
#include "movementtype.h"

#include "player.h"
#include "../world.h"
#include "../utils/confignode.h"
#include "../utils/drop.h"

#include <iostream>

Monster::Monster() : target(nullptr){}
Monster::~Monster(){}

void Monster::load(const ConfigNode& config){
	Creature::load(config);
	ConfigNode onHit = config.getNodeConst("on-hit");
	hitAttack.load(onHit);

	team = config.getInt("team", 1);

	movementType.reset(MovementType::staticLoad(config));

	dropList.reset(new DropList);
	const_cast<ConfigNode&>(config).getNode("drops").forEachNode([&](ConfigNode& node){
		dropList->emplace_back(node.getString("tag"));
	});
}

void Monster::logic(double time){
	if (!alive()){
		dropList->dropStuff(*world, pos);
		world->removeEntity(this);
		return;
	}

	cooldown -= time;
	Creature::logic(time);

	if (target != nullptr){
		movementType->moveTo(*this, target->pos, time);
	}

	if (cooldown.done()){
		Rect4d boundingBox = getBoundingBox();
		world->forEachCreature([&](Creature& creature){
			if (aggressiveTo(creature) && boundingBox.intersects(creature.getBoundingBox()))
				hitCreature(creature);
		});
	}
}

void Monster::hitCreature(Creature& other){
	other.takeDamage(hitAttack, pos);
	cooldown.set(0.5);
}

void Monster::hitTerrain(bool hitWall){
	movementType->hitTerrain(*this, hitWall);
}

void Monster::onCreatureDied(Creature* other){
	if (target == other)
		target = nullptr;
}
