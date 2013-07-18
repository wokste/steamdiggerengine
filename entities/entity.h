#pragma once
#include <string>
#include "../utils/vector2.h"
#include <memory>

class Attack;
class Texture;
class Entity;
class ConfigNode;
class World;

class Entity{
public:
	Entity();
	Entity(Entity& prototype, World* newWorld, Vector2d newPos);
	virtual void load(const ConfigNode& config);
	bool validPos(World& world, Vector2d newPos);
	bool isInArea(Vector2d px1, Vector2d px2);

	virtual ~Entity() {}
	void logic(double time);
	void render();
	void startAnim(std::string animName);
	void move(Vector2d change);
	void push(Vector2d dir, double force);
	Rect4d getBoundingBox() const;

	virtual void hitTerrain(bool hitWall);


public:
	// Variables
	Vector2d pos;
	World* world;

	Vector2d speed;
	int HP;

	double maxSpeed;
	bool bMapCollision;
	bool bGravity;
	Vector2i size;
	Vector2d frameOffset;
	Vector2d collision;
	std::shared_ptr<Texture> texture= nullptr;
	int HPMax;
};
