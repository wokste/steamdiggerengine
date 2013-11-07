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

#include "player.h"
#include "../world.h"
#include <pugixml.hpp>
#include "../utils/drop.h"

#include <iostream>

Monster::Monster() : target(nullptr){}
Monster::~Monster(){}

void Monster::load(pugi::xml_node& node){
	Creature::load(node);
	pugi::xml_node onHit = node.child("attack");
	hitAttack.load(onHit);

	team = node.attribute("team").as_int(1);

	dropList.reset(new DropList);
	auto dropNode = node.child("drops");
	dropList->load(dropNode);
	dropList->postLoad();
}

void Monster::logic(double time){
	if (!alive()){
		world->entities->remove(this);
		return;
	}

	cooldown -= time;
	Creature::logic(time);

	if (target != nullptr && (physicsMode == PhysicsMode::Walking || physicsMode == PhysicsMode::Jumping)){
		speed.x = (facing == Direction::left) ? -walkSpeed : walkSpeed;
	}

	if (cooldown.done()){
		Rect4d boundingBox = getBoundingBox();
		for (auto creature : world->creatures()){
			if (creature->alive() && aggressiveTo(creature) && boundingBox.intersects(creature->getBoundingBox()))
				hitCreature(*creature);
		}
	}
}

void Monster::takeDamage(const Attack& attack, Vector2d source){
	if (!alive())
		return;

	Creature::takeDamage(attack, source);

	if (!alive()){
		dropList->dropStuff(*world, pos, attack.type);
	}
}

void Monster::hitCreature(Creature& other){
	other.takeDamage(hitAttack, pos);
	cooldown.set(0.5);
}

void Monster::onCreatureDied(Creature* other){
	if (target == other)
		target = nullptr;
}

void Monster::hitTerrain(bool hitWall){
	if (hitWall){
		// Foodstep height
		if (physicsMode != PhysicsMode::Walking)
			speed.x = 0;
		else if (tryWallClimb(facing))
			speed.x = 0;
		else
			facing = (facing == Direction::left) ? Direction::right : Direction::left;
	}else{
		if (speed.y > 0){
			physicsMode = PhysicsMode::Walking;
		}
		speed.y = 0;
	}
}
