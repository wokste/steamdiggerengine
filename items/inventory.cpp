#include "inventory.h"
#include "itemdefmanager.h"
#include "itemdef.h"
#include "weapon.h"
#include <iostream>

Inventory::Inventory(ItemDefManager* newItemDefs) : itemDefs(newItemDefs){
	for (int i = 0; i < 36; ++i){
		items[i].id = i % 2 + 1;
		items[i].count = 10;
	}
	selectedItem=0;
}

bool Inventory::use(Player& owner, Vector2i mousePos){
	if (items[selectedItem].count == 0)
		return false;

	return itemDefs->getItemDef(items[selectedItem].id)->use(owner, items[selectedItem], mousePos);
}
