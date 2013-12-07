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
#include "src/entities/creature.h"
#include "src/utils/attack.h"
#include "src/utils/vector2.h"
#include <pugixml.hpp>
#include "src/world.h"
#include "src/map/map.h"


Creature::Creature(): team(0), facing(Direction::right){}

Creature::~Creature(){

}

void Creature::load(pugi::xml_node& configNode)
{
	Entity::load(configNode);
	HP = configNode.attribute("hp").as_int(100);
	shield = configNode.attribute("shield").as_int();
	jumpHeight = configNode.attribute("jump-height").as_double();
	accelSpeed = configNode.attribute("acceleration").as_double();
	walkSpeed = configNode.attribute("walk-speed").as_double();
}

void Creature::logic(double time)
{
	Entity::logic(time);
	if (alive()){
		regenCooldown -= time;
		while (regenCooldown.done()){
			shield.heal(1);
			regenCooldown.add(0.1);
		}
	}
}

void Creature::takeDamage(Creature* source, int damage, const int damageType){
	damage = shield.soak(damage);
	damage = HP.soak(damage);
	regenCooldown.set(5);
}

bool Creature::aggressiveTo(Creature* other){
	return alive() && other->alive() && team != other->team;
}

bool Creature::tryJump(int height){
	if (!alive() || physicsMode != PhysicsMode::Walking)
		return false;

	if (height == 0)
		height = jumpHeight;

	physicsMode = PhysicsMode::Jumping;
	speed.y = std::min(speed.y, -std::sqrt((2 * height + 1) * world->map->gravity));
	return true;
}

bool Creature::tryWallClimb(Direction climbDirection){
	if (physicsMode != PhysicsMode::Walking)
		return false;

	for (int blocksToJump = 1; blocksToJump <= jumpHeight; blocksToJump++){
		if (validPos(*world, Vector2d(pos.x + (climbDirection == Direction::right ? 0.5 : -0.5), pos.y - blocksToJump))){
			tryJump(blocksToJump);
			return true;
		}
	}
	return false;
}
