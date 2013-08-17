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
#include "movementtype.h"
#include "monster.h"
#include "player.h"
#include "../world.h"
#include <math.h>
#include "../utils/confignode.h"

// == FlyingMovement ==

struct FlyingMovement : public MovementType{
	double accelSpeed;
	double bounceSpeed;

	FlyingMovement(const ConfigNode& config);
	virtual void moveTo(Monster& self, Vector2d position, double time);
	virtual void hitTerrain(Monster& self, bool hitWall);
};

FlyingMovement::FlyingMovement(const ConfigNode& config){
	accelSpeed = config.getInt("acceleration");
	bounceSpeed = config.getInt("bounce-speed");
}

void FlyingMovement::moveTo(Monster& monster, Vector2d pos, double time){
	double t = 1;

	Vector2d move = (pos) - (monster.pos + monster.speed * t);

	monster.speed += Vector2::normalize(move) * time * accelSpeed;
}

/** The aim of this function is to find a possible path into houses for flying monsters.
  * This is done by moving in the general direction where the monster was flying.
  * Using this type of hopping, the AI hopefully finds it's way to a gate.
  * The hopping is randomised to avoid repeating failed attempts.
  */
void FlyingMovement::hitTerrain(Monster& monster, bool hitWall){
	double speedL = bounceSpeed * 1;//Rand.double(0.9, 1);
	double speedS = bounceSpeed * 0.2;//Rand.double(0.2, 0.3);

	if (hitWall){
		monster.speed.x = (monster.speed.x > 0) ? -speedS : speedS;
		monster.speed.y = (monster.speed.y > 0) ? speedL : -speedL;
	}
	else{
		monster.speed.x = (monster.speed.x > 0) ? speedL : -speedL;
		monster.speed.y = (monster.speed.y > 0) ? -speedS : speedS;
	}
}

// == Factory ==

MovementType* MovementType::staticLoad(const ConfigNode& config){
	return new FlyingMovement(config);
}
