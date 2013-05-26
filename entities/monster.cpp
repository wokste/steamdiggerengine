#include "monster.h"
#include "movementtype.h"

#include "player.h"
#include "../world.h"
#include "../utils/confignode.h"

#define STATS ((MonsterStats&)(stats))
#include <iostream>

// TODO: Hitwall

MonsterStats::MonsterStats(){}
MonsterStats::~MonsterStats(){}

void MonsterStats::load(const Game& game, const ConfigNode& config){
	EntityStats::load(game, config);
	ConfigNode onHit = config.getNodeConst("on-hit");
	hitAttack.load(onHit);

	movementType.reset(MovementType::staticLoad(game, config));
}

Monster* MonsterStats::spawn(World* world, Vector2d pos){
	if (!validPos(*world, pos))
		return nullptr;

	auto mob = new Monster(world, pos, *this);
	world->monsters.push_back(std::unique_ptr<Monster>(mob));
	return mob;
}

Monster::Monster(World* newWorld, Vector2d newPos, MonsterStats& newStats) : Entity(newWorld, newPos,newStats) , target(nullptr), cooldown(){

}

void Monster::takeDamage(Attack& attack,Vector2d source){
	HP -= attack.damage;
	push(pos - source, attack.push);
}

void Monster::logic(double time){
	cooldown -= time;
	Entity::logic(time);

	if (target != nullptr){
		STATS.movementType->moveTo(*this, target->pos, time);
	}

	if (cooldown.done()){
		Rect4d boundingBox = getBoundingBox();
		for (auto& player : world->players){
			if (boundingBox.intersects(player->getBoundingBox()))
				hitPlayer(*(player.get()));
		}
	}
}

void Monster::hitPlayer(Player& other){
	other.takeDamage(STATS.hitAttack, pos);
	cooldown.set(0.5);
}

void Monster::hitTerrain(bool hitWall){
	STATS.movementType->hitTerrain(*this, hitWall);
}
