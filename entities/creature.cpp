#include "creature.h"
#include <sstream>

#include "../attack.h"

#include "../utils/vector2.h"
#include "../utils/confignode.h"
#include "../game.h"

Stat::Stat(): cur(0), max(0){}
Stat::~Stat(){}

Stat::Stat(int newVal){
	cur = newVal;
	max = newVal;
}

/// returns the amount that is healed.
bool Stat::heal(int healing){
	if (cur == max)
		return false;

	cur = std::min(cur + healing, max);
	return true;
}

/// reduces stat
/// returns the amount that is not soaked.
int Stat::soak(int damage){
	cur -= damage;
	if (cur < 0){
		auto notSoaked = -cur;
		cur = 0;
		return notSoaked;
	}
	return 0;
}

double Stat::asProportion() const{
	if (max <= 0)
		return 1;
	return (double)cur / (double)max;
}

std::string Stat::asText() const{
	std::stringstream out;
	out << cur << " / " << max;
	return out.str();
}

Creature::Creature(){}
Creature::~Creature(){}

void Creature::load(const ConfigNode& config)
{
	Entity::load(config);
	HP = config.getInt("hp",100);
	shield = config.getInt("shield",0);
}

void Creature::logic(double time)
{
	Entity::logic(time);
	regenCooldown -= time;
	while (regenCooldown.done()){
		shield.heal(1);
		regenCooldown.add(0.1);
	}
}

void Creature::takeDamage(const Attack& attack,Vector2d source){
	int damage = attack.damage;

	damage = shield.soak(damage);
	damage = HP.soak(damage);
	push(pos - source, attack.push);

	regenCooldown.set(5);
}

bool Creature::aggressiveTo(Creature& other){
	return team != other.team;
}
