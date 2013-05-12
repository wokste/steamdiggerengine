#pragma once
#include <string>
#include "../utils/vector2.h"

class Attack;
class Texture;
class Entity;
class ConfigNode;
class World;
class Game;

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
	virtual void load(const Game& game, const ConfigNode& config);
	bool validPos(World& world, Vector2d newPos);
};

class Entity{
public:
	Vector2d pos;
	World* world;

	EntityStats* stats;
	Vector2d speed;
	double HP;

	Entity(World* world, Vector2d newPos, EntityStats* stats);
	bool isInArea(Vector2d px1, Vector2d px2);

	virtual ~Entity() {}
	void logic(double time);
	void render();
	void startAnim(std::string animName);
	void move(Vector2d change);
	void push(Vector2d dir, double force);
	Rect4d getRect() const;

	virtual void hitTerrain(bool hitWall);
	virtual void takeDamage(Attack& attack, Vector2d source);
};
