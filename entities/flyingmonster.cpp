#include "flyingmonster.h"
#include "../world.h"
#include <math.h>

#include <math.h>
#include "../utils/confignode.h"

#define STATS ((FlyingMonsterStats*)(stats))

Entity * FlyingMonsterStats::spawn(World& newWorld, Vector2d pos){
	return new FlyingMonster(newWorld, pos, this);
}

void FlyingMonsterStats::load(GameSettings& gameSettings, ConfigNode& config){
	MonsterStats::load(gameSettings, config);
	accelSpeed = config.getInt("acceleration");
	bounceSpeed = config.getInt("bounce-speed");
}

FlyingMonster::FlyingMonster(World& newWorld, Vector2d newPos, FlyingMonsterStats* newStats) : Monster(newWorld, newPos,newStats){
	entityType = EntityType::ET_FlyingMonster;
}

void FlyingMonster::logic(int timeMs){
	Monster::logic(timeMs);
	double timeS = timeMs / 1000.0;

	Entity* pTarget = target.get(*world->entities);

	if (pTarget != nullptr){
		double t = 1;

		double dx = (pTarget->pos.x + pTarget->speed.x * t) - (pos.x + speed.x * t);
		double dy = (pTarget->pos.y + pTarget->speed.y * t) - (pos.y + speed.y * t);
		double dtot = std::max(sqrt(dx * dx + dy * dy), 0.1);

		speed.x += (dx / dtot) * timeS * STATS->accelSpeed;
		speed.y += (dy / dtot) * timeS * STATS->accelSpeed;
	}
}

/** The aim of this function is to find a possible path into houses for flying monsters.
  * This is done by moving in the general direction where the monster was flying.
  * Using this type of hopping, the AI hopefully finds it's way to a gate.
  * The hopping is randomised to avoid repeating failed attempts.
  */
void FlyingMonster::hitTerrain(bool hitWall){
	double speedL = STATS->bounceSpeed * 1;//Rand.double(0.9, 1);
	double speedS = STATS->bounceSpeed * 0.2;//Rand.double(0.2, 0.3);

	if (hitWall){
		speed.x = (speed.x > 0) ? -speedS : speedS;
		speed.y = (speed.y > 0) ? speedL : -speedL;
	}
	else{
		speed.x = (speed.x > 0) ? speedL : -speedL;
		speed.y = (speed.y > 0) ? -speedS : speedS;
	}
}

