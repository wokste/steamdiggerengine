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

	FlyingMovement(const Game& game, const ConfigNode& config);
	virtual void moveTo(Monster& self, Vector2d position, double time);
	virtual void hitTerrain(Monster& self, bool hitWall);
};

FlyingMovement::FlyingMovement(const Game& game, const ConfigNode& config){
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

MovementType* MovementType::staticLoad(const Game& game, const ConfigNode& config){
	return new FlyingMovement(game,config);
}
