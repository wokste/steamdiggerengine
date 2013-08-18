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
#include "drop.h"
#include "../game.h"
#include "../items/itemdefmanager.h"
#include "../world.h"
#include "../entities/droppeditem.h"

#include <iostream>

void ItemReference::calcID(){
	if (tag == "")
		return;

	id = GameGlobals::itemDefs->at(tag);
	if (id < 0){
		std::cerr << "Inventory tag " << tag << " not found\n";
		return;
	}
	tag = "";
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
		if (item.ref.id >= 0){
			auto drop = new DroppedItem(item.ref.id, 1);
			drop->setPos(&world, pos);
			world.addEntity(drop);
		}
	}
}

void DropList::postLoad(){
	for(auto& item: *this){
		item.ref.calcID();
	}
}
