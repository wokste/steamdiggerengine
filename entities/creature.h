#pragma once
#include "entity.h"

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
	Creature(Creature& prototype, World* newWorld, Vector2d newPos);
	virtual void load(const ConfigNode& config);
	void takeDamage(const Attack& attack, Vector2d source);
	bool alive() {return HP.cur > 0;}
public:
	Stat HP;
	Stat shield;
};
