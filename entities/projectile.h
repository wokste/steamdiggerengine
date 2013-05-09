#pragma once
#include "entity.h"
#include "player.h"
#include "monster.h"
#include "../attack.h"

class Projectile;
class Player;
class Monster;

struct ProjectileStats : public EntityStats{
	Attack hitAttack;
	double speed;
	bool playerProjectile;

	ProjectileStats() = default;
	Projectile* spawn(World& world, Vector2d pos);
	virtual void load(const Game& game, const ConfigNode& config);
};

class Projectile : public Entity{
public:
	Projectile(World& world, Vector2d newPos, ProjectileStats * stats);
	virtual void hitTerrain(bool hitWall);
	void onCollision(Player& other);
	void onCollision(Monster& other);
	virtual void moveTo(Vector2d point);
private:
	void damage(Entity& other);
};
