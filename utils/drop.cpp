#include "drop.h"
#include "../game.h"
#include "../items/itemdefmanager.h"
#include "../world.h"
#include "../entities/droppeditem.h"

#include <iostream>

int ItemReference::getID() const{
	if (id < 0 && tag != "") {
		id = GameGlobals::itemDefs->at(tag);
		if (id < 0){
			std::cerr << "Inventory tag " << tag << " not found\n";
		}
		tag = "";
	}
	return id;
}

Drop::Drop(int id) : ref(id)
{
	chance = 1;
	minSize = 1;
	maxSize = 1;
}

Drop::Drop(std::string tag) : ref(tag)
{
	chance = 1;
	minSize = 1;
	maxSize = 1;
}

void DropList::dropStuff(World& world, Vector2d pos) const{
	for(auto& item: *this){
		// TODO: stack sizes
		// TODO: chances
		auto drop = new DroppedItem(item.ref.getID(), 1);
		drop->setPos(&world, pos);
		world.addEntity(drop);
	}
}
