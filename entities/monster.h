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
#include "src/entities/creature.h"
#include "src/attack.h"
#include "src/cooldown.h"
#include <memory>

class Player;
class MovementType;

class Monster : public Creature{
public:
	Monster();
	virtual ~Monster();

	void logic(double time) override;

	void load(pugi::xml_node& configNode) override;
	void hitCreature(Creature& other);
	void onCreatureDied(Creature* other) override;
	virtual void takeDamage(const Attack& attack, Vector2d source);
	void hitTerrain(bool hitWall) override;
public:
	Attack hitAttack;
	std::shared_ptr<DropList> dropList;
	Cooldown cooldown;

	Creature* target;
};
