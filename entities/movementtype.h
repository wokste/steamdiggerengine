#pragma once
#include "../utils/vector2.h"

class Game;
class ConfigNode;
class Monster;

struct MovementType{
	MovementType(){}
	static MovementType* staticLoad(const ConfigNode& config);
	virtual void moveTo(Monster& self, Vector2d position, double time) = 0;
	virtual void hitTerrain(Monster& self, bool hitWall) = 0;
};
