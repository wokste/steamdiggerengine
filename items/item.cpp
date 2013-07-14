#include "item.h"
#include "../entities/entity.h"
#include "../screen.h"
#include "../effects/effect.h"
#include "../utils/confignode.h"
#include "../enums.h"
#include <SFML/Window/Keyboard.hpp>

ItemType::ItemType(ConfigNode& config){
	maxStack=config.getInt("stack",1);
	consumable=config.getBool("consumable",false);
	useTime=config.getDouble("use-time",0.2);

	auto onUseConfig = config.getNode("on-use");
	onUseConfig.forEachNode([&] (ConfigNode& effect) {
		auto e = Effect::loadEffect(effect);
		if (e)
			onUseEffects.push_back(std::move(e));
	});
}

ItemType::ItemType(int frameID){
	maxStack=250;
	consumable=true;
	useTime=0.2;
}

ItemType::~ItemType(){
}

bool ItemType::use(Entity& user, const Screen& screen){
	int layer = Layer::front;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
		layer = Layer::back;

	bool success = false;
	for(auto& effect: onUseEffects){
		if (effect->use(user, user.pos, screen.mousePos(layer), layer))
			success = true;
	}
	return success;
}
