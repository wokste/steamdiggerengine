#include "itemdef.h"
#include "../utils/confignode.h"
#include "../screen.h"

ItemDef::ItemDef(ConfigNode& config){

}

bool ItemDef::use(Player& owner, ItemStack& itemStack, Screen& screen){
	return false;
}
