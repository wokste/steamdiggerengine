#include "itemdef.h"
#include "../utils/confignode.h"
#include "../screen.h"

ItemDef::ItemDef(const ConfigNode& config){

}

int ItemDef::use(Player& owner, ItemStack& itemStack, const Screen& screen){
	return 0;
}
