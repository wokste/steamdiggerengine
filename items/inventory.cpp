#include "inventory.h"
#include "itemdefmanager.h"
#include "item.h"
#include <iostream>
#include "../cooldown.h"
#include "../screen.h"
#include "../entities/player.h"
#include "../game.h"
#include "../entities/droppeditem.h"
#include "../world.h"

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
			world.addEntity(drop);
		}
	}
}
