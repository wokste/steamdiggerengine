#include "effect.h"
#include "mapeffect.h"
#include "combateffect.h"

#include "../utils/confignode.h"
#include <iostream>

std::unique_ptr<Effect> Effect::loadEffect(ConfigNode& config){
	std::unique_ptr<Effect> effect;
	std::string typeName = config.getString("type","");

	if (typeName == "mine")
		effect.reset(new MineEffect(config));
	if (typeName == "shoot")
		effect.reset(new ShootEffect(config));
	if (typeName == "heal")
		effect.reset(new HealEffect(config));

	return effect;
}


EffectSlot::EffectSlot(){

}

EffectSlot::~EffectSlot(){

}

void EffectSlot::load(ConfigNode& jsonArray){
	jsonArray.forEachNode([&] (ConfigNode& jsonEffect) {
		add(Effect::loadEffect(jsonEffect));
	});
}

void EffectSlot::add(std::unique_ptr<Effect> newEffect){
	if (newEffect)
		effects.push_back(std::move(newEffect));
}

bool EffectSlot::run(Entity& owner, Vector2d sourcePos, Vector2d targetPos, int targetLayer){
	bool success = false;
	for(auto& testedEffect: effects){
		if (testedEffect->run(owner, sourcePos, targetPos, targetLayer))
			success = true;
	}
	return success;
}
