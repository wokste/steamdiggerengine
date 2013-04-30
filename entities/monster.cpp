#include "monster.h"
#include "../world.h"
#include "../utils/confignode.h"
#include "entitylist.h"

#define STATS ((MonsterStats*)(stats))
#include <iostream>

void MonsterStats::load(Game& game, ConfigNode& config){
	EntityStats::load(game, config);
	ConfigNode onHit = config.getNode("on-hit");
	hitAttack.load(onHit);
}

Monster::Monster(World& newWorld, Vector2d newPos, MonsterStats* newStats) : Entity(newWorld, newPos,newStats) , target(0,0), cooldown(){

}

void Monster::takeDamage(Attack& attack,Vector2d source){
	HP -= attack.damage;
	push(pos - source, attack.push);

	if (HP <= 0)
		bDeleteMe = true;
}

void Monster::logic(int timeMs){
	cooldown -= timeMs;
	Entity::logic(timeMs);

	// TODO: this does not have to be calculated every frame
	target = world->entities->findMax([this](const Entity& val){
		if (val.entityType == EntityType::ET_Player && val.HP > 0)
			return 10000000.0;

		return 0.0;
		/*auto dx = x - val.x;
		auto dy = y - val.y;
		return 160000.0 - dx * dx + dy * dy; // 400 PX sight radius*/
	});
}


void Monster::onCollision(Entity& other){
	if (other.entityType == EntityType::ET_Player && cooldown.done()){
		other.takeDamage(STATS->hitAttack, pos);
		cooldown.set(500);
		std::cout << "hurts";
	}
}
