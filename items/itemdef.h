#pragma once
#include "../utils/vector2.h"

class Player;
class ItemStack;
class Block;
class ConfigNode;
class Screen;
class Game;

class ItemDef{
public:
	int ID;
	ItemDef(const ConfigNode& config);
	virtual double use(Player& owner, ItemStack& item, const Screen& screen);
	virtual Block* asBlock(){return nullptr;}
};
