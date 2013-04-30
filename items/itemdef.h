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
class Game;

class ItemDef{
public:
	int ID;
	ItemDef(const ConfigNode& config);
	virtual int use(Player& owner, ItemStack& item, const Screen& screen);
	virtual Block* asBlock(){return nullptr;}
};
