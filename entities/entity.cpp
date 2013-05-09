#include "entity.h"
#include "../utils/texture.h"
#include "../world.h"
#include "../attack.h"
#include "../map/map.h"

#include <math.h>
#include <algorithm>
#include "../utils/confignode.h"
#include "../utils/vector2.h"

#include "player.h"
#include "flyingmonster.h"
#include "projectile.h"
#include "../utils/assert.h"
#include "../game.h"

int floorInt(double);

EntityStats::EntityStats() :
	maxSpeed(0),
	bMapCollision(true),
	bGravity(true),
	texture(nullptr),
	HP(0),
	color(0xffffff),
	team(0)
{
	size = Vector2i(16,32);
	frameOffset = Vector2d(-1,-2);
	collision = Vector2d(1,2);
}

Entity::Entity(World& newWorld, Vector2d newPos, EntityStats* _stats) :
	stats(_stats),
	flipped(false),
	bDeleteMe(false),
	world(&newWorld)
{
	pos = newPos;
	speed = Vector2d(0,0);
	HP = stats->HP;
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
	if (speed.x > 0.5) flipped = false;
	if (speed.x < -0.5) flipped = true;

	if (stats->texture != nullptr){
		stats->texture->bind(stats->color);
		int frame = 0;

		//if (flipped)
		//	stats->texture->drawTile(Vector2d(pos.x + stats->frameOffset.x + stats->size.x / 16.0, pos.y + stats->frameOffset.y), Vector2i(-stats->size.x, stats->size.y), frame);
		//else
			stats->texture->drawTile(Vector2d(pos.x + stats->frameOffset.x, pos.y + stats->frameOffset.y), stats->size, frame);
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
	if (!stats->bMapCollision)
		return true;
	return !world->areaHasBlocks(Vector2::dToI(newPos - stats->collision), Vector2::dToI(newPos + stats->collision));
}

bool Entity::isInArea(Vector2d px1, Vector2d px2){
	return (
	   (pos.x + stats->collision.x > px1.x) &&
	   (pos.x - stats->collision.x < px2.x) &&
	   (pos.y + stats->collision.y > px1.y) &&
	   (pos.y - stats->collision.y < px2.y));
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
	if (dTot < 0.01)
		return; // Push too close

	speed += force * dir / dTot;
}

EntityStats::~EntityStats(){
	if (texture != nullptr)
		delete texture;
}

void EntityStats::load(const Game& game, const ConfigNode& config){
	maxSpeed = config.getDouble("max-speed");
	bMapCollision = config.getBool("mapcollison",true);
	bGravity = config.getBool("gravity",true);

	size = config.getVector2i("size");

	frameOffset =-(config.getVector2d("collision", 1) + config.getVector2d("collision", 0)) / 2.0;
	collision   = (config.getVector2d("collision", 1) - config.getVector2d("collision", 0)) / 2.0;
	const std::string textureName = config.getString("texture");
	texture = new Texture(game.fileSystem.fullpath(textureName), config.getVector2i("size"));
	HP = config.getInt("hp",100);
	color = config.getInt("color",0xffffff);
	team = config.getInt("team",1);
}
