#include <iostream>
#include <math.h>
#include "projectile.h"
#include "../attack.h"
#include "../utils/confignode.h"

#define STATS ((ProjectileStats *)(stats))

Projectile* ProjectileStats::spawn(World& newWorld, Vector2d pos){
	return new Projectile(newWorld, pos, this);
}

Projectile::Projectile(World& newWorld, Vector2d newPos, ProjectileStats * stats) : Entity(newWorld, newPos, stats){
}

void Projectile::hitTerrain(bool hitWall){
	bDeleteMe = true;
}

void Projectile::hitPlayer(Player& other){
	if (other.stats->team != stats->team){
		other.takeDamage(STATS->hitAttack, pos);
		bDeleteMe = true;
	}
}

void Projectile::hitMonster(Monster& other){
	if (other.stats->team != stats->team){
		other.takeDamage(STATS->hitAttack, pos);
		bDeleteMe = true;
	}
}

void Projectile::moveTo(Vector2d newPos){
	// TODO: Better aim for gravity affected projectiles

	Vector2d diff = newPos - pos;

	double dtot = std::max(sqrt(diff.x * diff.x + diff.y * diff.y), 0.1);
	speed = diff / dtot * STATS->speed;
}

void ProjectileStats::load(const Game& game, const ConfigNode& config){
	EntityStats::load(game, config);
	speed = config.getInt("speed");
	ConfigNode onHit = config.getNodeConst("on-hit");
	hitAttack.load(onHit);
	//TODO: on-hit-wall
}
