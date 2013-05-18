#include "itemdef.h"
#include "../utils/confignode.h"
#include "../screen.h"

ItemDef::ItemDef(const ConfigNode& config) : ID(-1){
	// The actual ID will be set by the ItemDefManager;
}

double ItemDef::use(Player& owner, ItemStack& itemStack, const Screen& screen){
	return 0;
}
