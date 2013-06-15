#pragma once
#include "effect.h"

class BuildEffect : public Effect{
public:
	int blockTypeID;
	BuildEffect(const ConfigNode& node);
	virtual bool use(Entity& owner, Vector2d sourcePos, Vector2d targetPos, int targetLayer);
};

class MineEffect : public Effect{
public:
	MineEffect(const ConfigNode& node);
	virtual bool use(Entity& owner, Vector2d sourcePos, Vector2d targetPos, int targetLayer);
};
