#pragma once
#include "entity.h"
//#include "movementtype.h"
#include "../attack.h"
#include "../cooldown.h"
#include <memory>

class Monster;
class Player;
class MovementType;

struct MonsterStats : public EntityStats{
	MonsterStats();
	~MonsterStats();

	Attack hitAttack;
	std::unique_ptr<MovementType> movementType;

	Monster* spawn(World* world, Vector2d pos);
	virtual void load(const Game& game, const ConfigNode& config);
};

class Monster : public Entity{
public:
	Cooldown cooldown;
	Monster(World* world, Vector2d newPos, MonsterStats* stats);
	virtual void logic(double time);
	virtual void hitPlayer(Player& other);
	virtual void takeDamage(Attack& attack, Vector2d source);
	Player* target;
	virtual void hitTerrain(bool hitWall);
};
