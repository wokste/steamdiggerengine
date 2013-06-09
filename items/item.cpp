#include "item.h"
#include "../entities/entity.h"
#include "../screen.h"
#include "../effects/effect.h"
#include "../utils/confignode.h"

ItemType::ItemType(ConfigNode& config){
	maxStack=config.getInt("stack",1);
	consumable=config.getBool("consumable",false);
	useTime=config.getDouble("use-time",1.0);

	auto onUseConfig = config.getNode("on-use");
	onUseConfig.forEachNode([&] (ConfigNode& effect) {
		auto e = Effect::loadEffect(effect);
		if (e)
			onUseEffects.push_back(std::move(e));
	});
}

ItemType::~ItemType(){
}

bool ItemType::use(Entity& user, const Screen& screen){
	bool success = false;
	for(auto& effect: onUseEffects){
		if (effect->use(user, screen))
			success = true;
	}
	return success;
}
