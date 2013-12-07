/*
The MIT License (MIT)

Copyright (c) 2013, Steven Wokke

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once
#include "src/utils/vector2.h"
#include "src/items/itemstack.h"
#include "src/utils/cooldown.h"
#include <SFML/Window.hpp>

class ItemDef;
class Player;
class ItemDefManager;
class Screen;
class InventoryHUD;
class World;

class Inventory{
	static constexpr int width = 10;
	static constexpr int height = 4;

	ItemStack items[width * height];
	int selectedItem;
	Cooldown cooldown;

	friend InventoryHUD;
public:
	ItemStack mouseHeldItem;

	Inventory();
	void logic(double time);
	bool use(Player& owner, const Screen& screen);
	void selectItem(int delta, bool relative = false);
	bool add(int itemId, int count = 1);
	bool add(std::string itemTag, int count = 1);
	void dropStuff(double proportion, World& world, Vector2d position);
};
