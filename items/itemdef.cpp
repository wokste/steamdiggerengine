#include "itemdef.h"
#include "../utils/confignode.h"
#include "../screen.h"

void ItemDef::load(ConfigNode& config){

}

bool ItemDef::use(Player& owner, ItemStack& itemStack, Screen& screen){
	return false;
}
