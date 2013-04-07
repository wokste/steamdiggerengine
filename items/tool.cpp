#include "tool.h"
#include "../entities/player.h"
#include "../entities/projectile.h"

#include "../world.h"
#include "../utils/confignode.h"
#include "../screen.h"

Tool::Tool(){
}

bool Tool::use(Player& owner, ItemStack& item, Screen& screen){
	// TODO: implement tool
	return false;
}

void Tool::load(ConfigNode& config){
	ItemDef::load(config);
}
