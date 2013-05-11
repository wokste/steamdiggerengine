#pragma once
#include "entity.h"
#include "player.h"
#include "monster.h"
#include "../attack.h"

class Projectile;
class Player;
class Monster;
enum class ProjectileState {Flying, Exploding, DeleteMe};
enum class ProjectileTargetType {TargetPlayer, TargetMonster};

struct ProjectileStats : public EntityStats{
	Attack hitAttack;
	double speed;

	ProjectileStats() = default;
	Projectile* spawn(World* world, Vector2d pos);
	virtual void load(const Game& game, const ConfigNode& config);
};

class Projectile : public Entity{
public:
	Projectile(World* world, Vector2d newPos, ProjectileStats * stats);
	virtual void hitTerrain(bool hitWall);
	virtual void logic(int timeMs);

	ProjectileState state;
	ProjectileTargetType targetType;
	void moveTo(Vector2d point);

private:
	void hitCreature(Entity& other);
	void damage(Entity& other);
};
