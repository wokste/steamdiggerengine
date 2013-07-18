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

class Projectile : public Entity{
public:
	Projectile();
	virtual ~Projectile();
	Projectile(Projectile& prototype, World* newWorld, Vector2d newPos);
	virtual void load(const ConfigNode& config);

	virtual void hitTerrain(bool hitWall);
	virtual void logic(double time);

	ProjectileState state;
	ProjectileTargetType targetType;
	void moveTo(Vector2d point);

	Attack hitAttack;
	double projectileSpeed;

	double bounce; // Negative is no bounce
	double TTL; // Time to live in seconds
private:
	void hitCreature(Entity& other);
	void damage(Entity& other);
};
