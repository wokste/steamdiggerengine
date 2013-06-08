#pragma once
#include "effect.h"

class MineEffect : public Effect{
public:
	MineEffect(ConfigNode& node);
	virtual bool use(Entity& owner, const Screen& screen);
};