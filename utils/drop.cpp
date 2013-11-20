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
#include "src/utils/drop.h"
#include "src/game.h"
#include "src/items/itemdefmanager.h"
#include "src/world.h"
#include "src/entities/droppeditem.h"
#include "src/utils/attack.h"

#include <iostream>

Drop::Drop(int id) : ref(id)
{
	chance = 1;
	count = 1;
}

Drop::Drop(const std::string& tag, const double newChance, const int newCount) : ref(tag)
{
	chance = newChance;
	count = newCount;
}

void DropList::dropStuff(World& world, Vector2d pos, int damageType) const{
	auto choice = std::generate_canonical<double, 20>(GameGlobals::rnd);
	for(auto& item: *this){
		if (item.ref.id < 0)
			continue;
		if (choice >= 0 && choice < item.chance){
			int count = item.count;
			if (count > 1){
				double rand = std::generate_canonical<double, 20>(GameGlobals::rnd);
				count = std::round(((count - 1) * 2) * rand) + 1; // Slightly randomize count
			}
			auto drop = new DroppedItem(item.ref.id, count);
			drop->setPos(&world, pos);
			world.entities->add(drop);
		}
		choice -= item.chance;
	}
}

void DropList::load(pugi::xml_node& dropNode){
	for (auto childNode : dropNode){
		emplace_back(childNode.attribute("tag").as_string(), childNode.attribute("odds").as_double(1.0), childNode.attribute("count").as_int(1));
	}
}

void DropList::postLoad(){
	for(auto& item: *this){
		item.ref.postLoad();
	}
}
