#include "item.h"
#include "../entities/entity.h"
#include "../screen.h"
#include "../effects/effect.h"
#include "../utils/confignode.h"

ItemType::ItemType(ConfigNode& configArray){
	configArray.forEachNode([&] (ConfigNode& config) {
		auto e = Effect::loadEffect(config);
		if (e)
			effects.push_back(std::move(e));
	});
}

bool ItemType::use(Entity& user, const Screen& screen){
	bool success = false;
	for(auto& effect: effects){
		if (effect->use(user, screen))
			success = true;
	}
	return success;
}
