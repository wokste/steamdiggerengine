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
#include "src/items/inventory.h"
#include "src/items/itemdefmanager.h"
#include "src/items/item.h"
#include <iostream>
#include "src/cooldown.h"
#include "src/screen.h"
#include "src/entities/player.h"
#include "src/game.h"
#include "src/entities/droppeditem.h"
#include "src/world.h"

ItemStack::ItemStack(){
	id = 0;
	count = 0;
}

Inventory::Inventory() : cooldown(){
	selectedItem=0;
}

bool Inventory::use(Player& owner, const Screen& screen){
	if (!cooldown.done() || items[selectedItem].count <= 0)
		return false;

	ItemType& type = (*GameGlobals::itemDefs)[items[selectedItem].id];
	if (type.use(owner, screen)){
		cooldown.set(type.useTime);
		if (type.consumable)
			items[selectedItem].count--;

		return true;
	}
	return false;
}

void Inventory::logic(double time){
	cooldown -= time;
}

void Inventory::selectItem(int nr, bool relative){
	// Switching inventory
	if (relative)
		selectedItem = (selectedItem + nr + width) % width;
	else
		selectedItem = nr;
}

bool Inventory::add(std::string itemTag, int count){
	return add(GameGlobals::itemDefs->at(itemTag), count);
}

bool Inventory::add(int itemId, int count){
	if (itemId < 0)
		return false;

	for (auto& i: items){
		if (i.id == itemId && i.count > 0){
			if (i.count + count <= (*GameGlobals::itemDefs)[i.id].maxStack){
				i.count += count;
				return true;
			} else {
				count -= (*GameGlobals::itemDefs)[i.id].maxStack - i.count;
				i.count = (*GameGlobals::itemDefs)[i.id].maxStack;
			}
		}
	}

	// Find empty slots
	for (auto& i: items){
		if (i.count <= 0){
			i.id = itemId;
			i.count = count;
			return true;
		}
	}

	return false;
}

void Inventory::dropStuff(double proportion, World& world, Vector2d position){
	for (auto& i: items){
		int dropCount = i.count * proportion;
		if (dropCount > 0){
			i.count -= dropCount;

			auto drop = new DroppedItem(i.id, dropCount);
			drop->setPos(&world, position);
			world.entities->add(drop);
		}
	}
}
