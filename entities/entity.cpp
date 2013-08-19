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
#include "entity.h"
#include "../utils/texture.h"
#include "../world.h"
#include "../attack.h"
#include "../map/map.h"

#include <cmath>
#include <algorithm>
#include "../utils/confignode.h"
#include "../utils/vector2.h"
#include "../utils/assert.h"
#include "../utils/mathplus.h"
#include "../game.h"
#include "../enums.h"

int floorInt(double);

Entity::Entity() :
	maxSpeed(0),
	bMapCollision(true),
	physicsMode(PhysicsMode::Falling),
	world(nullptr)
{
	size = Vector2i(16,32);
	frameOffset = Vector2d(-1,-2);
	collision = Vector2d(1,2);
	isPlayer = false;
}

void Entity::setPos(World* newWorld, Vector2d newPos){
	pos = newPos;
	world = newWorld;
}

void Entity::logic(double time){
	if (physicsMode == PhysicsMode::Walking
		&& !world->areaHasBlocks(Vector2::dToI(pos + Vector2d(-collision.x, collision.y)), Vector2::dToI(pos + Vector2d(collision.x, collision.y + 1))))
	{
		physicsMode = PhysicsMode::Falling;
	}

	if (physicsMode == PhysicsMode::Falling){
		speed.y += world->map->gravity * time;
	}

	if (speed.x < -maxSpeed) speed.x = -maxSpeed;
	if (speed.x > maxSpeed) speed.x = maxSpeed;
	if (speed.y < -maxSpeed) speed.y = -maxSpeed;
	if (speed.y > maxSpeed) speed.y = maxSpeed;

	move(speed * time);
}

void Entity::render(const sf::Color& skyColor){
	if (texture != nullptr){
		texture->bind();
		glColor3ub(skyColor.r,skyColor.g,skyColor.b);
		texture->draw(Vector2i(0,0),size,Vector2d(pos.x + frameOffset.x, pos.y + frameOffset.y),Vector2d(size.x / 16.0,size.y / 16.0));
	}
}

Rect4d Entity::getBoundingBox() const{
	return Rect4d(pos - collision, collision + collision);
}

void Entity::startAnim(std::string animName){
	//animation.start(entityType->getAnimation(animName));
}

/// Moves the entity. Checks for collision in the process.
void Entity::move(Vector2d movement){
	pos.y += movement.y;
	if (!validPos(*world, pos)){
		if (movement.y > 0)
			// TODO: Fix
			pos.y = MathPlus::floorInt(pos.y + collision.y) - collision.y - 0.01;
		else
			pos.y = MathPlus::floorInt(pos.y - collision.y) + collision.y + 1;
		hitTerrain(false); // Hit a floor/ceiling
	}

	pos.x += movement.x;
	if (!validPos(*world, pos)){
		if (movement.x > 0)
			// TODO: Fix
			pos.x = MathPlus::floorInt(pos.x + collision.x) - collision.x - 0.01;
		else
			pos.x = MathPlus::floorInt(pos.x - collision.x) + collision.x + 1;
		hitTerrain(true); // Hit a wall
	}
}

bool Entity::validPos(World& world, Vector2d newPos){
	if (!bMapCollision)
		return true;
	return !world.areaHasBlocks(Vector2::dToI(newPos - collision), Vector2::dToI(newPos + collision));
}

bool Entity::isInArea(Vector2d px1, Vector2d px2){
	return (
	   (pos.x + collision.x > px1.x) &&
	   (pos.x - collision.x < px2.x) &&
	   (pos.y + collision.y > px1.y) &&
	   (pos.y - collision.y < px2.y));
}

void Entity::hitTerrain(bool hitWall){
	if (hitWall){
		speed.x = 0;
	}else{
		if (speed.y > 0){
			physicsMode = PhysicsMode::Walking;
		}
		speed.y = 0;
	}
}

void Entity::push(Vector2d dir , double force){
	speed = force * Vector2::normalize(dir);
	if (physicsMode == PhysicsMode::Walking){
		physicsMode = PhysicsMode::Falling;
		speed.y = std::min(speed.y, -force);
	}
}

void Entity::load(const ConfigNode& config){
	maxSpeed      = config.getDouble("max-speed");
	bMapCollision = config.getBool("mapcollison",true);
	size          = config.getVector2i("size");
	frameOffset   =-(config.getVector2d("collision", 1) + config.getVector2d("collision", 0)) / 2.0;
	collision     = (config.getVector2d("collision", 1) - config.getVector2d("collision", 0)) / 2.0;
	const std::string textureName = config.getString("texture");
	texture.reset(new Texture(GameGlobals::fileSystem.fullpath(textureName)));

	std::map<std::string,PhysicsMode> modeLookupTable = {{"none",PhysicsMode::None},{"walking",PhysicsMode::Walking},{"flying",PhysicsMode::Flying},{"falling",PhysicsMode::Falling}};
	physicsMode   = modeLookupTable[config.getString("physics", "falling")];
}
