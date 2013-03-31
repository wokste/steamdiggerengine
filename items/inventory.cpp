#include "inventory.h"
#include "itemdefmanager.h"
#include "itemdef.h"
#include "weapon.h"
#include <iostream>
#include "../cooldown.h"

Inventory::Inventory(ItemDefManager* newItemDefs) : itemDefs(newItemDefs), cooldown(){
	for (int i = 0; i < width * height; ++i){
		items[i].id = i % 2 + 1;
		items[i].count = 5;
	}
	selectedItem=0;
}

void Inventory::logic(int timeMs, Player& owner, Vector2i mousePos){
	cooldown -= timeMs;
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		return;

	if (!cooldown.done() || items[selectedItem].count <= 0)
		return;

	if (itemDefs->getItemDef(items[selectedItem].id)->use(owner, items[selectedItem], mousePos)){
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
