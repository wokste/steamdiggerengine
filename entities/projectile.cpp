#include <iostream>
#include <math.h>
#include "projectile.h"
#include "../attack.h"
#include "../world.h"
#include "../utils/confignode.h"

#define STATS ((ProjectileStats *)(stats))

Projectile* ProjectileStats::spawn(World* world, Vector2d pos){
	if (!validPos(*world, pos))
		return nullptr;

	auto projectile = new Projectile(world, pos, this);
	world->projectiles.push_back(std::unique_ptr<Projectile>(projectile));
	return projectile;
}

Projectile::Projectile(World* newWorld, Vector2d newPos, ProjectileStats * stats) :
	Entity(newWorld, newPos, stats),
	state(ProjectileState::Flying)
{}

void Projectile::logic(int timeMs){
	Entity::logic(timeMs);

	// Check for collisions
	if (state == ProjectileState::Flying){
		Rect4d rect = getRect();
		if (targetType == ProjectileTargetType::TargetPlayer){
			for (auto& player : world->players){
				if (rect.intersects(player->getRect()))
					hitCreature(*(player.get()));
			}
		}
		if (targetType == ProjectileTargetType::TargetMonster){
			for (auto& monster : world->monsters){
				if (rect.intersects(monster->getRect()))
					hitCreature(*(monster.get()));
			}
		}
	};
};

void Projectile::hitTerrain(bool hitWall){
	state = ProjectileState::DeleteMe;
}

void Projectile::hitCreature(Entity& other){
	other.takeDamage(STATS->hitAttack, pos);
	state = ProjectileState::DeleteMe;
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
