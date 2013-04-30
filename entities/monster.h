#pragma once
#include "entity.h"
#include "../attack.h"
#include "../cooldown.h"
#include "entityhandle.h"

struct MonsterStats : public EntityStats{
	Attack hitAttack;

	MonsterStats() = default;
	virtual void load(Game& game, ConfigNode& config);
};

class Monster : public Entity{
public:
	Cooldown cooldown;
	Monster(World& world, Vector2d newPos, MonsterStats* stats);
	virtual void logic(int timeMs);
	virtual void onCollision(Entity& other);
	virtual void takeDamage(Attack& attack, Vector2d source);
protected:
	EntityHandle target;
};
