#pragma once
#include "creature.h"
#include "../attack.h"
#include "../cooldown.h"
#include <memory>

class Player;
class MovementType;

class Monster : public Creature{
public:
	Monster();
	virtual ~Monster();

	void logic(double time) override;

	void load(const ConfigNode& config) override;
	void hitTerrain(bool hitWall) override;
	void hitCreature(Creature& other);
	void onCreatureDied(Creature* other) override;
public:
	Attack hitAttack;
	std::shared_ptr<MovementType> movementType;
	Cooldown cooldown;

	Creature* target;
};
