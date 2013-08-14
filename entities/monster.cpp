#include "monster.h"
#include "movementtype.h"

#include "player.h"
#include "../world.h"
#include "../utils/confignode.h"
#include "../utils/drop.h"

#include <iostream>

Monster::Monster() : target(nullptr){}
Monster::~Monster(){}

void Monster::load(const ConfigNode& config){
	Creature::load(config);
	ConfigNode onHit = config.getNodeConst("on-hit");
	hitAttack.load(onHit);

	team = config.getInt("team", 1);

	movementType.reset(MovementType::staticLoad(config));

	dropList.reset(new DropList);
	const_cast<ConfigNode&>(config).getNode("drops").forEachNode([&](ConfigNode& node){
		dropList->emplace_back(node.getString("tag"));
	});
}

void Monster::logic(double time){
	if (!alive()){
		dropList->dropStuff(*world, pos);
		world->removeEntity(this);
		return;
	}

	cooldown -= time;
	Creature::logic(time);

	if (target != nullptr){
		movementType->moveTo(*this, target->pos, time);
	}

	if (cooldown.done()){
		Rect4d boundingBox = getBoundingBox();
		for (auto& creature : world->creatures){
			if (aggressiveTo(*creature) && boundingBox.intersects(creature->getBoundingBox()))
				hitCreature(*creature);
		}
	}
}

void Monster::hitCreature(Creature& other){
	other.takeDamage(hitAttack, pos);
	cooldown.set(0.5);
}

void Monster::hitTerrain(bool hitWall){
	movementType->hitTerrain(*this, hitWall);
}

void Monster::onCreatureDied(Creature* other){
	if (target == other)
		target = nullptr;
}
