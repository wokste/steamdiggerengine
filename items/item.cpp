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
	auto onUseNode = config.getNode("on-use");
	onUse.load(onUseNode);
	frameID=config.getInt("frame");
}

ItemType::ItemType(int newFrameID){
	maxStack=250;
	consumable=true;
	useTime=0.2;
	frameID=newFrameID;
}

ItemType::~ItemType(){
}

bool ItemType::use(Entity& user, const Screen& screen){
	int layer = screen.getSelectedLayer();
	return onUse.run(user, user.pos, screen.mousePos(layer), layer);
}
