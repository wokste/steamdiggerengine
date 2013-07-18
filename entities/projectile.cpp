#include <iostream>
#include <math.h>
#include "projectile.h"
#include "../attack.h"
#include "../world.h"
#include "../utils/confignode.h"

Projectile::Projectile(){}
Projectile::~Projectile(){}

Projectile::Projectile(Projectile& prototype, World* world, Vector2d pos) : Entity(prototype, world, pos){
	hitAttack = prototype.hitAttack;
	projectileSpeed = prototype.projectileSpeed;

	bounce = prototype.bounce;
	TTL = prototype.TTL;
}

void Projectile::logic(double time){
	TTL -= time;
	if (TTL < 0)
		state = ProjectileState::DeleteMe;

	Entity::logic(time);

	// Check for collisions
	if (state == ProjectileState::Flying){
		Rect4d boundingBox = getBoundingBox();
		if (targetType == ProjectileTargetType::TargetPlayer){
			for (auto& player : world->players){
				if (boundingBox.intersects(player->getBoundingBox()))
					hitCreature(*(player.get()));
			}
		}
		if (targetType == ProjectileTargetType::TargetMonster){
			for (auto& monster : world->monsters){
				if (boundingBox.intersects(monster->getBoundingBox()))
					hitCreature(*(monster.get()));
			}
		}
	};
};

void Projectile::hitTerrain(bool hitWall){
	if (bounce >= 0){
		if (hitWall)
			speed.x = -speed.x * bounce;
		else
			speed.y = -speed.y * bounce;
	} else {
		state = ProjectileState::DeleteMe;
	}
}

void Projectile::hitCreature(Creature& other){
	other.takeDamage(hitAttack, pos);
	state = ProjectileState::DeleteMe;
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
