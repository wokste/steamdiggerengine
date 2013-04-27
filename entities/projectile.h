#pragma once
#include "entity.h"
#include "../attack.h"

struct ProjectileStats : public EntityStats{
	Attack hitAttack;
	double speed;

	ProjectileStats() = default;
	virtual Entity* spawn(World& world, Vector2d pos);
	virtual void load(GameSettings& gameSettings, ConfigNode& config);
};

class Projectile : public Entity{
public:
	Projectile(World& world, Vector2d newPos, ProjectileStats * stats);
	virtual void hitTerrain(bool hitWall);
	virtual void onCollision(Entity& other);
	virtual void moveTo(Vector2d point);
};
