#include <iostream>
#include <math.h>
#include "projectile.h"
#include "../attack.h"
#include "../world.h"
#include "../utils/confignode.h"

#define STATS ((ProjectileStats&)(stats))

Projectile* ProjectileStats::spawn(World* world, Vector2d pos){
	if (!validPos(*world, pos))
		return nullptr;

	auto projectile = new Projectile(world, pos, *this);
	world->projectiles.push_back(std::unique_ptr<Projectile>(projectile));
	return projectile;
}

Projectile::Projectile(World* newWorld, Vector2d newPos, ProjectileStats& stats) :
	Entity(newWorld, newPos, stats),
	state(ProjectileState::Flying)
{
	targetType = ProjectileTargetType::TargetPlayer;
}

void Projectile::logic(double time){
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
	state = ProjectileState::DeleteMe;
}

void Projectile::hitCreature(Entity& other){
	other.takeDamage(STATS.hitAttack, pos);
	state = ProjectileState::DeleteMe;
}

void Projectile::moveTo(Vector2d targetPos){
	// TODO: Better aim for gravity affected projectiles
	speed = Vector2::normalize(targetPos - pos) * STATS.speed;
}

void ProjectileStats::load(const Game& game, const ConfigNode& config){
	EntityStats::load(game, config);
	speed = config.getInt("speed");
	ConfigNode onHit = config.getNodeConst("on-hit");
	hitAttack.load(onHit);
	//TODO: on-hit-wall
}
