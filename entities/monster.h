#pragma once
#include "entity.h"
#include "../attack.h"
#include "../cooldown.h"
#include <memory>

class Player;
class MovementType;

class Monster : public Entity{
public:
	Monster();
	virtual ~Monster();
	Monster(Monster& prototype, World* newWorld, Vector2d newPos);

	virtual void logic(double time);
	virtual void hitPlayer(Player& other);
	virtual void takeDamage(Attack& attack, Vector2d source);
	virtual void load(const ConfigNode& config);
	virtual void hitTerrain(bool hitWall);
public:
	Attack hitAttack;
	std::shared_ptr<MovementType> movementType;
	Cooldown cooldown;

	Player* target;
};
