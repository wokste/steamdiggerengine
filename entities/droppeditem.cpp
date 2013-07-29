#include "droppeditem.h"
#include "creature.h"
#include "../world.h"
#include "../utils/texture.h"
#include "../utils/vector2.h"
#include "../game.h"
#include "../items/itemdefmanager.h"
#include "../items/item.h"

DroppedItem::DroppedItem(int itemId, int count) :
	Entity(),
	itemId(itemId),
	count(count)
{
	TTL = 5 * 60; // 5 minutes in seconds
	bGravity = true;
	maxSpeed = 200;
	bMapCollision = true;
	texture = GameGlobals::tileSet;
	collision = Vector2d(0.25,0.25);
	isPlayer =  false;
	frameOffset = collision * -1.0;
	size = Vector2i(32,32);
	speed = Vector2d(rand() % 8 - 4 ,rand() % 8 - 4);
}

DroppedItem::~DroppedItem(){

}

void DroppedItem::render(const sf::Color& skyColor){
	if (texture != nullptr){
		texture->bind();
		glColor3ub(skyColor.r,skyColor.g,skyColor.b);
		int framesPerRow = texture->size.x / 32;
		int frame = (*GameGlobals::itemDefs)[itemId].frameID;
		texture->draw(Vector2i((frame % framesPerRow), (frame / framesPerRow)) * 32, size, Vector2d(pos.x + frameOffset.x, pos.y + frameOffset.y),collision * 2.0);
	}
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

void DroppedItem::hitTerrain(bool hitWall){
	if (hitWall){
		speed.x = -speed.x;
	} else {
		if (speed.y > 0)
			speed.x = 0;
		speed.y = 0;
	}
}
