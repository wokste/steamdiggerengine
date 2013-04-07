#pragma once
#include "itemdef.h"
#include <string>
#include "enums.h"

class Tool : public ItemDef{
public:
	Tool();

	virtual bool use(Player& owner, ItemStack& item, Screen& screen);
	virtual void load(ConfigNode& config);
};
