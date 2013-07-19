#pragma once
#include "entity.h"
#include "../cooldown.h"

class Attack;
class Texture;
class Entity;
class ConfigNode;
class World;

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
	virtual void load(const ConfigNode& config);
	virtual void logic(double time);
	void takeDamage(const Attack& attack, Vector2d source);
	bool alive() {return HP.cur > 0;}
public:
	Stat HP;
	Stat shield;
	Cooldown regenCooldown;
};
