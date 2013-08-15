#include <iostream>
#include <math.h>
#include "projectile.h"
#include "../attack.h"
#include "../world.h"
#include "../utils/confignode.h"

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

void Projectile::load(const ConfigNode& config){
	Entity::load(config);
	projectileSpeed = config.getInt("speed");
	ConfigNode onHit = config.getNodeConst("on-hit");
	hitAttack.load(onHit);
	TTL = config.getDouble("ttl", 5);
	bounce = config.getDouble("bounce", -1);
}
