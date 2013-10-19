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
#include "creature.h"
#include <sstream>
#include "../attack.h"
#include "../utils/vector2.h"
#include <pugixml.hpp>
#include "../game.h"
#include "../world.h"
#include "../map/map.h"
#include <math.h>

Stat::Stat(): cur(0), max(0){}
Stat::~Stat(){}

Stat::Stat(int newVal){
	cur = newVal;
	max = newVal;
}

/// returns the amount that is healed.
bool Stat::heal(int healing){
	if (cur == max)
		return false;

	cur = std::min(cur + healing, max);
	return true;
}

/// reduces stat
/// returns the amount that is not soaked.
int Stat::soak(int damage){
	cur -= damage;
	if (cur < 0){
		auto notSoaked = -cur;
		cur = 0;
		return notSoaked;
	}
	return 0;
}

double Stat::asProportion() const{
	if (max <= 0)
		return 1;
	return (double)cur / (double)max;
}

std::string Stat::asText() const{
	std::stringstream out;
	out << cur << " / " << max;
	return out.str();
}

Creature::Creature(): team(0){}

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

void Creature::takeDamage(const Attack& attack,Vector2d source){
	int damage = attack.damage;

	damage = shield.soak(damage);
	damage = HP.soak(damage);
	push(pos - source, attack.push);

	regenCooldown.set(5);
}

bool Creature::aggressiveTo(Creature* other){
	return alive() && other->alive() && team != other->team;
}

void Creature::tryJump(int height){
	//if (alive() && physicsMode == PhysicsMode::Walking){
		if (height == 0)
			height = jumpHeight;

		double force = std::sqrt((2 * height + 1) * world->map->gravity);
		push(Vector2d(0,-1), force);
	//}
}

void Creature::hitTerrain(bool hitWall){
	if (hitWall){
		// Foodstep height
		//if (physicsMode == PhysicsMode::Walking){
			for (int blocksToJump = 1; blocksToJump <= jumpHeight; blocksToJump++){
				if (validPos(*world, Vector2d(pos.x + (speed.x > 0 ? 0.5 : -0.5), pos.y - blocksToJump))){
					tryJump(blocksToJump);
					break;
				}
			}
		//}
		speed.x = 0;
	}else{
		if (speed.y > 0){
			physicsMode = PhysicsMode::Walking;
		}
		speed.y = 0;
	}
}
