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
#include "entitylist.h"
#include "entity.h"
#include "../game.h"
#include "../utils/skybox.h"
#include "../map/map.h"
#include "creature.h"
#include "../world.h"
#include <stdlib.h>
#include <pugixml.hpp>
#include <SFML/Graphics/Color.hpp>
#include <iostream>
#include <assert.h>

EntityList::EntityList()
{
}

EntityList::~EntityList(){
	for (auto entity: entities){
		delete entity;
	}
}

void EntityList::logic(double time, World& world){
	for (auto& entity: entities){
		entity->logic(time);

		if (entity->isPlayer)
			world.map->generateAround(entity->pos);
	}

	// == Removal of entities ==
	for (auto& removing: toDelete){
		entities.erase(std::remove(entities.begin(), entities.end(), removing),entities.end());

		auto removingCreature = dynamic_cast<Creature*>(removing);
		if (removingCreature != nullptr){
			creatures.erase(std::remove(creatures.begin(), creatures.end(), removingCreature),creatures.end());
			for (auto entity: entities)
				entity->onCreatureDied(removingCreature);
		}

		delete removing;
	}
	toDelete.clear();

	// == Addition of entities ==
	for (auto& adding: toAdd){
		auto creature = dynamic_cast<Creature*>(adding);

		if (creature != nullptr){
			creatures.push_back(creature);
		}
		entities.push_back(adding);
	}
	toAdd.clear();
}

void EntityList::render(const Screen& screen, const Map& map, sf::Color dayLightColor){
	for (auto& entity : entities){
		sf::Color color = map.getColor(dayLightColor, entity->pos);
		entity->render(color);
	}
}

void EntityList::add(Entity* entity){
	toAdd.push_back(entity);
}

void EntityList::remove(Entity* entity){
	if (std::find(toDelete.begin(), toDelete.end(), entity)==toDelete.end())
		toDelete.push_back(entity);
}

std::vector<Entity*>::iterator EntityList::begin(){
	return entities.begin();
}

std::vector<Entity*>::iterator EntityList::end(){
	return entities.end();
}

EntityListCreatureView EntityList::getCreatures(){
	return EntityListCreatureView(this);
}

EntityListCreatureView::EntityListCreatureView(EntityList* newList) : list(newList){}

std::vector<Creature*>::iterator EntityListCreatureView::begin(){
	return list->creatures.begin();
}

std::vector<Creature*>::iterator EntityListCreatureView::end(){
	return list->creatures.end();
}

// Deprecated
bool EntityListCreatureView::areaHasCreature(Vector2i px1, Vector2i px2){
	Vector2d px1d = Vector2::iToD(px1);
	Vector2d px2d = Vector2::iToD(px2);
	for (auto creature : list->creatures)
		if (creature->isInArea(px1d, px2d)) return true;
	return false;
}
