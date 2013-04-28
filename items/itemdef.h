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
class GameSettings;

class ItemDef{
public:
	int ID;
	ItemDef(ConfigNode& config);
	virtual bool use(Player& owner, ItemStack& item, Screen& screen);
	virtual Block* asBlock(){return nullptr;}
};
