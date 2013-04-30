#pragma once
#include <string>
#include "../utils/vector2.h"

class Attack;
class Texture;
class Entity;
class ConfigNode;
class World;
class Game;

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
	Vector2d frameOffset;
	Vector2d collision;
	Texture * texture= nullptr;
	int HP = 0;
	int color = 0xffffff;
	int team = 0;

	EntityStats();
	virtual ~EntityStats();
	virtual Entity* spawn(World& world, Vector2d pos) = 0;
	virtual void load(Game& game, ConfigNode& config);

	static EntityStats* staticLoad(Game& game, ConfigNode& config);
};

class Entity{
public:
	Vector2d pos;
	World* world;

	EntityStats* stats;
	Vector2d speed;
	double HP;
	bool flipped;
	bool bDeleteMe;
	EntityType entityType;

	Entity(World& world, Vector2d newPos, EntityStats* stats);
	bool isInArea(Vector2d px1, Vector2d px2);

	virtual ~Entity() {}
	virtual void logic(int timeMs);
	virtual void render();
	virtual void startAnim(std::string animName);
	virtual void move(Vector2d change);
	virtual bool validPos(Vector2d newPos);
	virtual void onCollision(Entity& other);
	virtual void hitTerrain(bool hitWall);
	virtual void takeDamage(Attack& attack, Vector2d source);
	virtual void push(Vector2d dir, double force);
};
