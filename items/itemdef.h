#pragma once
#include "../utils/vector2.h"

class Player;
class ItemStack;
class Block;
class Weapon;
class Armour;
class Tool;
class ConfigNode;

class ItemDef{
public:
	int ID;
	virtual bool use(Player& owner, ItemStack& item, Vector2i mousePos) {return false;}
	virtual void load(ConfigNode& config);
};
