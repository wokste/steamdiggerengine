#pragma once
#include <string>
#include "../utils/vector2.h"

class Attack;
class Texture;
class Entity;
class ConfigNode;

enum class EntityType{
	ET_FlyingMonster,
	ET_Projectile,
	ET_Player,
	ET_Unknown
};

struct EntityStats{
	double maxSpeed=0;
	bool bMapCollision=true;
	bool bGravity=true;
	Vector2i size;
	Vector2i frameOffset;
	Vector2i collision;
	Texture * texture= nullptr;
	int hP = 0;
	int color = 0xffffff;
	int team = 0;

	EntityStats();
	virtual ~EntityStats();
	virtual Entity* spawn(Vector2d pos) = 0;
	virtual void load(ConfigNode& config);
};

class Entity{
public:
	Vector2d pos;
	EntityStats* stats;
	Vector2d speed;
	double HP;
	bool flipped;
	bool bDeleteMe;
	EntityType entityType;

	Entity(Vector2d newPos, EntityStats* stats);

	virtual ~Entity() {}
	virtual void logic(int timeMs);
	virtual void render();
	virtual void startAnim(std::string animName);
	virtual void move(Vector2d change);
	virtual bool validPos(Vector2d newPos);
	virtual bool checkCollision(Entity& other);
	virtual void onCollision(Entity& other);
	virtual void hitTerrain(bool hitWall);
	virtual void takeDamage(Attack& attack, Vector2d source);
	virtual void push(Vector2d dir , double force);
};
