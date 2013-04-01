#pragma once
#include "itemdef.h"
#include <string>
#include "enums.h"

class Tool : public ItemDef{
public:
	Tool();

	virtual bool use(Player& owner, ItemStack& item, Vector2i mousePos);
	virtual void load(ConfigNode& config);
};
