#include "itemdef.h"
#include "../utils/confignode.h"
#include "../screen.h"

ItemDef::ItemDef(ConfigNode& config){

}

int ItemDef::use(Player& owner, ItemStack& itemStack, Screen& screen){
	return 0;
}
