#pragma once
#include "itemdef.h"
#include <string>
#include <memory>
class ProjectileStats;

// WARNING: Deprecated class
class Gun : public ItemDef{
public:
	Gun(const Game& game, const ConfigNode& config);
	~Gun();

	std::unique_ptr<ProjectileStats> projectile;
	virtual double use(Player& owner, ItemStack& item, const Screen& screen);
};
