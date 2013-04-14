#include "inventory.h"
#include "itemdefmanager.h"
#include "itemdef.h"
#include "weapon.h"
#include <iostream>
#include "../cooldown.h"
#include "../screen.h"

Inventory::Inventory(ItemDefManager* newItemDefs) : itemDefs(newItemDefs), cooldown(){
	for (int i = 0; i < width * height; ++i){
		items[i].id = i % 7;
		items[i].count = 50;
	}
	selectedItem=0;
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
