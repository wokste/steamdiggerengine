#pragma once
#include <string>
#include "../utils/vector2.h"
#include <memory>

class Attack;
class Texture;
class Entity;
class ConfigNode;
class World;
class Creature;

class Entity{
public:
	Entity();
	virtual ~Entity() {}

	virtual void load(const ConfigNode& config);
	bool validPos(World& world, Vector2d newPos);
	void setPos(World* newWorld, Vector2d newPos);
	bool isInArea(Vector2d px1, Vector2d px2);

	virtual void logic(double time);
	void render();
	void startAnim(std::string animName);
	void move(Vector2d change);
	void push(Vector2d dir, double force);
	Rect4d getBoundingBox() const;

	virtual void hitTerrain(bool hitWall);
	virtual void onCreatureDied(Creature* other){}


public:
	// Variables
	Vector2d pos;
	World* world;

	Vector2d speed;

	double maxSpeed;
	bool bMapCollision;
	bool bGravity;
	Vector2i size;
	Vector2d frameOffset;
	Vector2d collision;
	std::shared_ptr<Texture> texture= nullptr;
};
