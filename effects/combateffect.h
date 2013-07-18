#pragma once
#include "effect.h"

#include <memory>
class Projectile;

class ShootEffect : public Effect{
public:
	ShootEffect(const ConfigNode& node);
	virtual bool run(Entity& owner, Vector2d sourcePos, Vector2d targetPos, int targetLayer);
private:
	std::unique_ptr<Projectile> projectile;
};

class HealEffect : public Effect{
public:
	HealEffect(const ConfigNode& node);
	virtual bool run(Entity& owner, Vector2d sourcePos, Vector2d targetPos, int targetLayer);
private:
	int hpGain;
};
