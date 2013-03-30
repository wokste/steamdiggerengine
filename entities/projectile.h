#pragma once
#include "entity.h"
#include "../attack.h"

struct ProjectileStats : public EntityStats{
	Attack hitAttack;
	double speed;

	ProjectileStats() = default;
	virtual Entity* spawn(Vector2d newPos);
	virtual void load(ConfigNode& config);
};

class Projectile : public Entity{
public:
	Projectile(Vector2d newPos, ProjectileStats * stats);
	virtual void hitTerrain(bool hitWall);
	virtual void onCollision(Entity& other);
	virtual void moveTo(Vector2d point);
};
