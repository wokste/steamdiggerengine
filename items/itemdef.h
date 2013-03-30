#pragma once
#include "../utils/vector2.h"

class Player;
class ItemStack;

class ItemDef{

public:
	virtual bool use(Player& owner, ItemStack& item, Vector2i mousePos) {return false;}
};
