#include "monster.h"
#include "movementtype.h"

#include "player.h"
#include "../world.h"
#include "../utils/confignode.h"

#include <iostream>

Monster::Monster(){}
Monster::~Monster(){}

// TODO: Hitwall

void Monster::load(const ConfigNode& config){
	Entity::load(config);
	ConfigNode onHit = config.getNodeConst("on-hit");
	hitAttack.load(onHit);

	movementType.reset(MovementType::staticLoad(config));
}

Monster::Monster(Monster& prototype, World* newWorld, Vector2d newPos) : Entity(prototype, newWorld, newPos) , target(nullptr), cooldown(){
	hitAttack = prototype.hitAttack;
	movementType = prototype.movementType;
	target = nullptr;
}

void Monster::takeDamage(Attack& attack,Vector2d source){
	HP -= attack.damage;
	push(pos - source, attack.push);
}

void Monster::logic(double time){
	cooldown -= time;
	Entity::logic(time);

	if (target != nullptr){
		movementType->moveTo(*this, target->pos, time);
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
	other.takeDamage(hitAttack, pos);
	cooldown.set(0.5);
}

void Monster::hitTerrain(bool hitWall){
	movementType->hitTerrain(*this, hitWall);
}
