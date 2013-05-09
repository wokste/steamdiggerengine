#pragma once
#include "monster.h"

struct FlyingMonsterStats : public MonsterStats{
	double accelSpeed;
	double bounceSpeed;

	FlyingMonsterStats() = default;
	virtual Monster* spawn(World& world, Vector2d pos);
	virtual void load(const Game& game, const ConfigNode& config);
};

class FlyingMonster : public Monster{
public:
	FlyingMonster(World& world, Vector2d newPos, FlyingMonsterStats* stats);
	virtual void logic(int timeMs);
	virtual void hitTerrain(bool hitWall);
};
