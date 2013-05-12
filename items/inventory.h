#pragma once
#include "../utils/vector2.h"
#include "../cooldown.h"
#include <SFML/Window.hpp>

class ItemDef;
class Player;
class ItemDefManager;
class Screen;

struct ItemStack{
	int id;
	int count;
};

class Inventory{
	constexpr static int width = 9;
	constexpr static int height = 4;

	ItemStack items[width * height];
	int selectedItem;
	ItemDefManager* itemDefs;
	Cooldown cooldown;
public:
	Inventory(ItemDefManager* newItemDefs);
	void logic(double time);
	bool use(Player& owner, const Screen& screen);
	void selectItem(int delta, bool relative = false);
	bool add(int itemId, int count = 1);
};
