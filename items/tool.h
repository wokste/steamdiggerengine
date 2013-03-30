#pragma once
#include "itemdef.h"
#include <string>

class Tool : public ItemDef{
public:
	Tool();

	virtual bool use(Player& owner, ItemStack& item, Vector2i mousePos);
};
