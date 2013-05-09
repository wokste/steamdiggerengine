#include "monster.h"
#include "player.h"
#include "../world.h"
#include "../utils/confignode.h"

#define STATS ((MonsterStats*)(stats))
#include <iostream>

void MonsterStats::load(const Game& game, const ConfigNode& config){
	EntityStats::load(game, config);
	ConfigNode onHit = config.getNodeConst("on-hit");
	hitAttack.load(onHit);
}

Monster::Monster(World& newWorld, Vector2d newPos, MonsterStats* newStats) : Entity(newWorld, newPos,newStats) , target(nullptr), cooldown(){

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
	target = world->players[0].get();
	// TODO: Fix
	// TODO: this does not have to be calculated every frame
	/*target = world->players->findMax([this](const Entity& val){
		if (val.entityType == EntityType::ET_Player && val.HP > 0)
			return 10000000.0;

		return 0.0;
		//auto dx = x - val.x;
		//auto dy = y - val.y;
		//return 160000.0 - dx * dx + dy * dy; // 400 PX sight radius
	});*/
}

void Monster::onCollision(Player& other){
	if (cooldown.done()){
		other.takeDamage(STATS->hitAttack, pos);
		cooldown.set(500);
	}
}
