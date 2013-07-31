#pragma once
#include "entity.h"
#include "../cooldown.h"

#include <memory>

class Attack;
class Texture;
class Entity;
class ConfigNode;
class World;
class DropList;

struct Stat{
	int cur;
	int max;

	Stat();
	Stat(int newVal);
	Stat(const Stat& other) = default;
	~Stat();
	bool heal(int healing);
	int soak(int damage);

	double asProportion() const;
	std::string asText() const;
};

class Creature : public Entity{
public:
	Creature();
	virtual ~Creature();
	void load(const ConfigNode& config) override;
	void logic(double time) override;
	void takeDamage(const Attack& attack, Vector2d source);
	bool alive() {return HP.cur > 0;}
	bool aggressiveTo(Creature& other);
	virtual bool pickupItem(int id, int count){return false;}
public:
	Stat HP;
	Stat shield;
	Cooldown regenCooldown;
	int team;
};
