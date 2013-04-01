#include "tool.h"
#include "../entities/player.h"
#include "../entities/projectile.h"

#include "../world.h"
#include "../utils/confignode.h"

Tool::Tool(){
}

bool Tool::use(Player& owner, ItemStack& item, Vector2i mousePos){
	// TODO: implement tool
	return false;
}

void Tool::load(ConfigNode& config){
	ItemDef::load(config);
}
