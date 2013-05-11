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

	if (cooldown.done()){
		Rect4d rect = getRect();
		for (auto& player : world->players){
			if (rect.intersects(player->getRect()))
				hitPlayer(*(player.get()));
		}
	}
}

void Monster::hitPlayer(Player& other){
	other.takeDamage(STATS->hitAttack, pos);
	cooldown.set(500);
}
