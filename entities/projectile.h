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
#include "player.h"
#include "monster.h"
#include "../attack.h"

class Projectile;
class Player;
class Monster;
enum class ProjectileState {Flying, Exploding, DeleteMe};

class Projectile : public Entity{
public:
	Projectile();
	virtual ~Projectile();
	virtual void load(const ConfigNode& config);

	virtual void hitTerrain(bool hitWall);
	virtual void logic(double time);

	ProjectileState state;
	int team;
	void moveTo(Vector2d point);

	Attack hitAttack;
	double projectileSpeed;

	double bounce; // Negative is no bounce
	double TTL; // Time to live in seconds
private:
	void hitCreature(Creature& other);
};
