#include "inventory.h"
#include "itemdefmanager.h"
#include "itemdef.h"
#include "weapon.h"
#include <iostream>
#include "../cooldown.h"
#include "../screen.h"

Inventory::Inventory(ItemDefManager* newItemDefs) : itemDefs(newItemDefs), cooldown(){
	for (int i = 0; i < width * height; ++i){
		items[i].id = 0;
		items[i].count = 0;
	}
	selectedItem=0;

	items[0].id = 4; items[0].count = 1; // Mining tool
	items[1].id = 6; items[1].count = 1; // Gun
}

void Inventory::logic(int timeMs, Player& owner, Screen& screen){
	cooldown -= timeMs;
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		return;

	if (!cooldown.done() || items[selectedItem].count <= 0)
		return;

	if (itemDefs->getItemDef(items[selectedItem].id)->use(owner, items[selectedItem], screen)){
		cooldown.set(200);
	}
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
