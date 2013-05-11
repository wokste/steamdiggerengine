#pragma once
#include "entity.h"
#include "../attack.h"
#include "../cooldown.h"

class Player;

struct MonsterStats : public EntityStats{
	Attack hitAttack;

	MonsterStats() = default;
	virtual void load(const Game& game, const ConfigNode& config);
};

class Monster : public Entity{
public:
	Cooldown cooldown;
	Monster(World& world, Vector2d newPos, MonsterStats* stats);
	virtual void logic(int timeMs);
	virtual void hitPlayer(Player& other);
	virtual void takeDamage(Attack& attack, Vector2d source);
	Player* target;
};
