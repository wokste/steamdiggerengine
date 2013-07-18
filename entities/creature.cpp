#include "creature.h"
#include <sstream>

#include "../attack.h"

#include "../utils/vector2.h"
#include "../utils/confignode.h"
#include "../game.h"

Stat::Stat(){}
Stat::~Stat(){}

Stat::Stat(int newVal){
	cur = newVal;
	max = newVal;
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

Creature::Creature(Creature& prototype, World* newWorld, Vector2d newPos) : Entity(prototype, newWorld, newPos){
	HP = prototype.HP;
	shield = prototype.shield;
}

void Creature::load(const ConfigNode& config)
{
	Entity::load(config);
	HP = config.getInt("hp",100);
	shield = config.getInt("shield",0);
}

void Creature::takeDamage(Attack& attack,Vector2d source){
	// TODO: shield
	HP.cur -= attack.damage;
	push(pos - source, attack.push);
}
