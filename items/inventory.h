#pragma once
#include "../utils/vector2.h"
#include "../cooldown.h"
#include <SFML/Window.hpp>

class ItemDef;
class Player;
class ItemDefManager;
class Screen;
class InventoryHUD;

struct ItemStack{
	ItemStack();
	int id;
	int count;
};

class Inventory{
	constexpr static int width = 10;
	constexpr static int height = 4;

	ItemStack items[width * height];
	int selectedItem;
	Cooldown cooldown;

	friend InventoryHUD;
public:
	Inventory();
	void logic(double time);
	bool use(Player& owner, const Screen& screen);
	void selectItem(int delta, bool relative = false);
	bool add(int itemId, int count = 1);
};
