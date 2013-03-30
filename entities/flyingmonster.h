#pragma once
#include "monster.h"

struct FlyingMonsterStats : public MonsterStats{
	double accelSpeed;
	double bounceSpeed;

	FlyingMonsterStats() = default;
	virtual Entity * spawn(Vector2d);
	virtual void load(ConfigNode& config);
};

class FlyingMonster : public Monster{
public:
	FlyingMonster(Vector2d newPos, FlyingMonsterStats* stats);
	virtual void logic(int timeMs);
	virtual void hitTerrain(bool hitWall);
};
