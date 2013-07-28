#include "inventory.h"
#include "itemdefmanager.h"
#include "item.h"
#include <iostream>
#include "../cooldown.h"
#include "../screen.h"
#include "../entities/player.h"
#include "../game.h"

ItemStack::ItemStack(){
	id = 0;
	count = 0;
}

Inventory::Inventory() : cooldown(){
	selectedItem=0;

	for (int i = 0; i < 2; i++){
		add(i, (*GameGlobals::itemDefs)[i].maxStack);
	}
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

bool Inventory::add(int itemId, int count){
	for (int i = 0; i < width * height; ++i){
		if (items[i].id == itemId && items[i].count > 0){
			if (items[i].count + count <= (*GameGlobals::itemDefs)[items[i].id].maxStack){
				items[i].count += count;
				return true;
			} else {
				count -= (*GameGlobals::itemDefs)[items[i].id].maxStack - items[i].count;
				items[i].count = (*GameGlobals::itemDefs)[items[i].id].maxStack;
			}
		}
	}

	// Find empty slots
	for (int i = 0; i < width * height; ++i){
		if (items[i].count <= 0){
			items[i].id = itemId;
			items[i].count = count;
			return true;
		}
	}

	return false;
}
