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
#include <iostream>
#include <math.h>
#include "projectile.h"
#include "../attack.h"
#include "../world.h"
#include <pugixml.hpp>

Projectile::Projectile(){
	bounce = 0;
	TTL = 0;
	projectileSpeed = 0;
	team = 0;
	state = ProjectileState::Flying;
}
Projectile::~Projectile(){}

void Projectile::logic(double time){
	TTL -= time;
	if (TTL < 0)
		world->removeEntity(this);

	Entity::logic(time);

	// Check for collisions
	if (state == ProjectileState::Flying){
		Rect4d boundingBox = getBoundingBox();
		world->forEachCreature([&](Creature& creature){
			if (team != creature.team && boundingBox.intersects(creature.getBoundingBox())){
				hitCreature(creature);
				return;
			}
		});
	}
}

void Projectile::hitTerrain(bool hitWall){
	if (bounce >= 0){
		if (hitWall)
			speed.x = -speed.x * bounce;
		else
			speed.y = -speed.y * bounce;
	} else {
		world->removeEntity(this);
	}
}

void Projectile::hitCreature(Creature& other){
	other.takeDamage(hitAttack, pos);
	world->removeEntity(this);
}

void Projectile::moveTo(Vector2d targetPos){
	// TODO: Better aim for gravity affected projectiles
	speed = Vector2::normalize(targetPos - pos) * projectileSpeed;
}

void Projectile::load(pugi::xml_node& configNode){
	Entity::load(configNode);
	projectileSpeed = configNode.attribute("speed").as_int();
	hitAttack.load(configNode);
	TTL = configNode.attribute("ttl").as_double(5);
	bounce = configNode.attribute("bounce").as_double(-1);
}
