#include "droppeditem.h"
#include "creature.h"
#include "../world.h"

DroppedItem::DroppedItem(int itemId, int count) :
	Entity(),
	itemId(itemId),
	count(count)
{

}

DroppedItem::~DroppedItem(){
}

void DroppedItem::logic(double time){
	TTL -= time;
	if (TTL < 0)
		world->removeEntity(this);

	Entity::logic(time);

	// Check for collisions
	Rect4d boundingBox = getBoundingBox();
	for (auto& creature : world->creatures){
		if (boundingBox.intersects(creature->getBoundingBox()) && creature->pickupItem(itemId, count)){
			world->removeEntity(this);
			return;
		}
	}
}
