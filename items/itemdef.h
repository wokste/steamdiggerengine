#pragma once
#include "../utils/vector2.h"
#include <iostream>

class Player;
class ItemStack;
class ConfigNode;
class Screen;
class Game;

class ItemDef{
public:
	int ID;
	ItemDef(const ConfigNode& config);
	virtual double use(Player& owner, ItemStack& item, const Screen& screen);
};
