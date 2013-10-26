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
#include "entity.h"
#include "../cooldown.h"
#include "../enums.h"

#include <memory>

class Attack;
class Texture;
class Entity;

class World;
class DropList;

struct Stat{
	int cur;
	int max;

	Stat();
	Stat(int newVal);
	Stat(const Stat& other) = default;
	~Stat();
	bool heal(int healing);
	int soak(int damage);

	double asProportion() const;
	std::string asText() const;
};

class Creature : public Entity{
public:
	Creature();
	virtual ~Creature();
	void load(pugi::xml_node& configNode) override;
	void logic(double time) override;
	virtual void takeDamage(const Attack& attack, Vector2d source);
	bool alive() {return HP.cur > 0;}
	bool aggressiveTo(Creature* other);
	virtual bool pickupItem(int id, int count){return false;}

	// Moves
	bool tryJump(int height = 0);
	bool tryWallClimb(Direction climbDirection);
public:
	Stat HP;
	Stat shield;
	Cooldown regenCooldown;
	int team;

	double jumpHeight;
	double accelSpeed;
	double walkSpeed;
	Direction facing;
};
