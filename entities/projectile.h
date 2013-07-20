#pragma once
#include "entity.h"
#include "player.h"
#include "monster.h"
#include "../attack.h"

class Projectile;
class Player;
class Monster;
enum class ProjectileState {Flying, Exploding, DeleteMe};

class Projectile : public Entity{
public:
	Projectile();
	virtual ~Projectile();
	virtual void load(const ConfigNode& config);

	virtual void hitTerrain(bool hitWall);
	virtual void logic(double time);

	ProjectileState state;
	int team;
	void moveTo(Vector2d point);

	Attack hitAttack;
	double projectileSpeed;

	double bounce; // Negative is no bounce
	double TTL; // Time to live in seconds
private:
	void hitCreature(Creature& other);
};
