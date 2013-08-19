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
#pragma once
#include <string>
#include "../utils/vector2.h"
#include <SFML/Graphics/Color.hpp>
#include <memory>

class Attack;
class Texture;
class Entity;
class ConfigNode;
class World;
class Creature;

enum class PhysicsMode{
	None,
	Walking,
	Flying,
	Falling
};

class Entity{
public:
	Entity();
	virtual ~Entity() {}

	virtual void load(const ConfigNode& config);
	bool validPos(World& world, Vector2d newPos);
	void setPos(World* newWorld, Vector2d newPos);
	bool isInArea(Vector2d px1, Vector2d px2);

	virtual void logic(double time);
	virtual void render(const sf::Color& skyColor);
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
	PhysicsMode physicsMode;
	Vector2i size;
	Vector2d frameOffset;
	Vector2d collision;
	std::shared_ptr<Texture> texture= nullptr;
	bool isPlayer;
};
