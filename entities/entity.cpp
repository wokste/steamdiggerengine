#include "entity.h"
#include "../texture.h"
#include "../world.h"
#include "../attack.h"
#include "../map.h"

#include <math.h>
#include <algorithm>
#include "../utils/confignode.h"
#include "../utils/vector2.h"

int floorInt(double);

EntityStats::EntityStats() :
	maxSpeed(0),
	bMapCollision(true),
	bGravity(true),
	texture(nullptr),
	hP(0),
	color(0xffffff),
	team(0)
{
	size = Vector2i(16,32);
	frameOffset = Vector2i(0,0);
	collision = Vector2i(16,32);
}

Entity::Entity(Vector2d newPos, EntityStats * _stats) :
	stats(_stats),
	flipped(false),
	bDeleteMe(false),
	entityType(EntityType::ET_Unknown)
{
	pos = newPos;
	speed = Vector2d(0,0);
	HP = stats->hP;
}

void Entity::logic(int timeMs){
	double timeS = timeMs / 1000.0;

	if (stats->bGravity)
		speed.y += world->map->gravity * timeS;

	if (speed.x < -stats->maxSpeed) speed.x = -stats->maxSpeed;
	if (speed.x > stats->maxSpeed) speed.x = stats->maxSpeed;
	if (speed.y < -stats->maxSpeed) speed.y = -stats->maxSpeed;
	if (speed.y > stats->maxSpeed) speed.y = stats->maxSpeed;

	move(speed * timeS);

	//if (animation.animate(timeMs))
	//	onAnimEnd();
}

/// Rendering function
void Entity::render(){
	if (speed.x > 10) flipped = false;
	if (speed.x < -10) flipped = true;

	if (stats->texture != nullptr){
		stats->texture->bind(stats->color);
		int frame = 0;

		if (flipped)
			stats->texture->drawTile(Vector2i(floorInt(pos.x) + stats->frameOffset.x + stats->size.x, floorInt(pos.y) + stats->frameOffset.y), Vector2i(-stats->size.x, stats->size.y), frame);
		else
			stats->texture->drawTile(Vector2i(floorInt(pos.x) + stats->frameOffset.x, floorInt(pos.y) + stats->frameOffset.y), stats->size, frame);
	}
}

/// Starts an animation
void Entity::startAnim(std::string animName){
	//animation.start(entityType->getAnimation(animName));
}

/// Moves the entity. Checks for collision in the process.
void Entity::move(Vector2d movement){
	if (validPos(Vector2d(pos.x, pos.y + movement.y))){
		pos.y += movement.y;
	}else{
		hitTerrain(false); // Hit no wall
	}

	if (validPos(Vector2d(pos.x + movement.x, pos.y))){
		pos.x += movement.x;
	}else{
		hitTerrain(true); // Hit a wall
	}
}

bool Entity::validPos(Vector2d newPos){
	if (!stats->bMapCollision) return true;
	int xint = floorInt(newPos.x);
	int yint = floorInt(newPos.y);
	return world->validPos(xint - stats->collision.x, xint + stats->collision.x, yint - stats->collision.y, yint + stats->collision.y);
}

bool Entity::checkCollision(Entity& other){
	return (
	   (pos.x + stats->collision.x > other.pos.x - other.stats->collision.y) &&
	   (pos.x - stats->collision.x < other.pos.x + other.stats->collision.y) &&
	   (pos.y + stats->collision.x > other.pos.y - other.stats->collision.y) &&
	   (pos.y - stats->collision.x < other.pos.y + other.stats->collision.y));
}

void Entity::onCollision(Entity& other){

}

void Entity::hitTerrain(bool hitWall){
	if (hitWall)
		speed.x = 0;
	else
		speed.y = 0;
}

void Entity::takeDamage(Attack& attack, Vector2d source) {}

void Entity::push(Vector2d dir , double force){
	double dTot = sqrt(dir.x * dir.x + dir.y * dir.y);
	if (dTot < 0.1)
		return; // Push too close

	speed += force * dir / dTot;
}

EntityStats::~EntityStats(){
	if (texture != nullptr)
		delete texture;
}

void EntityStats::load(ConfigNode& config){
	maxSpeed = config.getInt("max-speed");
	bMapCollision = config.getBool("mapcollison",true);
	bGravity = config.getBool("gravity",true);

	size = config.getVector2i("size");

	frameOffset = -(config.getVector2i("size", 0) + config.getVector2i("size", 1)) / 2;
	collision  = (config.getVector2i("size", 0) - config.getVector2i("size", 1)) / 2;

	texture=Texture::load(config);
	hP = config.getInt("hp",100);
	color = config.getInt("color",0xffffffff);
	if (color > 0x1000000) color += 0xff000000; // Alpha value
	team = config.getInt("team",1);
}
