#include "inventory.h"
#include "itemdefmanager.h"
#include "itemdef.h"
#include "gun.h"
#include <iostream>
#include "../cooldown.h"
#include "../screen.h"

ItemStack::ItemStack(){
	id = 0;
	count = 0;
}

Inventory::Inventory(ItemDefManager& newItemDefs) : itemDefs(newItemDefs), cooldown(){
	selectedItem=0;

	items[0].id = 2; items[0].count = 1; // Mining tool
	items[1].id = 3; items[1].count = 1; // Gun
	items[2].id = 4; items[2].count = 5; // Lanterns
}
bool Inventory::use(Player& owner, const Screen& screen){
	if (!cooldown.done() || items[selectedItem].count <= 0)
		return false;
	double time = itemDefs[items[selectedItem].id]->use(owner, items[selectedItem], screen);
	if (time > 0){
		cooldown.set(time);
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
			items[i].count += count;
			return true;
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
