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
#include "src/entities/droppeditem.h"
#include "src/entities/creature.h"
#include "src/world.h"
#include "src/utils/texture.h"
#include "src/utils/vector2.h"
#include "src/game.h"
#include "src/items/itemdefmanager.h"
#include "src/items/item.h"

DroppedItem::DroppedItem(int itemId, int count) :
	Entity(),
	itemId(itemId),
	count(count)
{
	TTL = 5 * 60; // 5 minutes in seconds
	physicsMode = PhysicsMode::Falling;
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
		world->entities->remove(this);

	Entity::logic(time);

	// Check for collisions
	Rect4d boundingBox = getBoundingBox();
	for (auto creature : world->creatures()){
		if (creature->alive() && boundingBox.intersects(creature->getBoundingBox()) && creature->pickupItem(itemId, count)){
			world->entities->remove(this);
			break;
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
