#include "item.h"
#include "../entities/entity.h"
#include "../screen.h"
#include "../effects/effect.h"
#include "../utils/confignode.h"
#include "../enums.h"
#include <SFML/Window/Keyboard.hpp>

ItemType::ItemType(ConfigNode& config, const Game& game){
	maxStack=config.getInt("stack",1);
	consumable=config.getBool("consumable",false);
	useTime=config.getDouble("use-time",0.2);
	auto onUseNode = config.getNode("on-use");
	onUse.load(onUseNode, game);
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

	return onUse.run(user, user.pos, screen.mousePos(layer), layer);
}
