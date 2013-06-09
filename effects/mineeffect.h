#pragma once
#include "effect.h"

class MineEffect : public Effect{
public:
	MineEffect(const ConfigNode& node);
	virtual bool use(Entity& owner, const Screen& screen);
};
