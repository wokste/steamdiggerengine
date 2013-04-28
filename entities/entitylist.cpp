#include "entitylist.h"
#include "entity.h"
#include "flyingmonster.h"
#include "player.h"
#include "projectile.h"
#include "../utils/assert.h"
#include <tuple>
#include <iostream>

ListItem::ListItem(unsigned int newNr, Entity* newEntity){
	nr = newNr;
	entity = newEntity;
}

EntityList::EntityList(){
}

EntityList::~EntityList(){
	foreach([&](int id, ListItem& item) {
		delete item.entity;
		item.entity = nullptr;
		item.nr = 0;
	});
}

void EntityList::foreach(std::function<void (int id, ListItem& e)> func){
	auto numEntities = entities.size();
	for (unsigned int i = 0 ; i < numEntities; ++i){
		if (entities[i].exists()){
			func(i, entities[i]);
		}
	}
}

EntityHandle EntityList::findMax(std::function<double (const Entity&)> func){
	double curMax = 0;
	EntityHandle curHandle(0,0);

	foreach([&](int id, const ListItem& item) {
		double eMax = func(*item.entity);
		if (eMax > curMax){
			curMax = eMax;
			curHandle.index = id;
			curHandle.nr = item.nr;
		}
	});
	return curHandle;
}

EntityHandle EntityList::addEntity(Entity* pE){
	static int autoIncrementNumber = 1;

	auto numEntities = entities.size();
	for (unsigned int i = 0 ; i < numEntities; ++i){
		if (!entities[i].exists()){
			entities[i].entity = pE;
			entities[i].nr = ++autoIncrementNumber;
			return EntityHandle(i, entities[i].nr);
		}
	}
	int nr = ++autoIncrementNumber;
	entities.push_back(ListItem(nr, pE));
	return EntityHandle(numEntities, nr);
}

void EntityList::logic(int timeMs){
	//std::vector<Entity *> toDelete;

	// == Basic Logic ==
	foreach([&](int id, ListItem& item) {
		item.entity->logic(timeMs);
		if (item.entity->bDeleteMe){
			delete item.entity;
			item.entity = nullptr;
			item.nr = 0;
		}
	});

	// == Check Collisions ==
	foreach([&](int, ListItem& item1) {
		Entity* entity1 = item1.entity;
		foreach([&](int, ListItem& item2) {
			Entity* entity2 = item2.entity;
			if (entity1->isInArea(entity2->pos - entity2->stats->collision, entity2->pos + entity2->stats->collision)){
				entity1->onCollision(*entity2);
				entity2->onCollision(*entity1);
			}
		});
	});
}

bool EntityList::areaHasEntity(Vector2i px1, Vector2i px2){
	bool hasEntity = false;
	foreach([&](int, ListItem& item) {
		if (item.entity->isInArea(Vector2::iToD(px1), Vector2::iToD(px2))){
			hasEntity = true;
		}
	});
	return hasEntity;
}

void EntityList::render(){
	foreach([&](int, ListItem& item) {
		item.entity->render();
	});
}
