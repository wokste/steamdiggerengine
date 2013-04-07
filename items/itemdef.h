#pragma once
#include "../utils/vector2.h"

class Player;
class ItemStack;
class Block;
class Weapon;
class Armour;
class Tool;
class ConfigNode;
class Screen;

class ItemDef{
public:
	int ID;
	virtual bool use(Player& owner, ItemStack& item, Screen& screen);
	virtual void load(ConfigNode& config);
};
