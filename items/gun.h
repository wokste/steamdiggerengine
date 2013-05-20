#pragma once
#include "itemdef.h"
#include <string>
class ProjectileStats;

class Gun : public ItemDef{
public:
	Gun(const Game& game, const ConfigNode& config);
	~Gun();

	ProjectileStats* projectile;
	virtual double use(Player& owner, ItemStack& item, const Screen& screen);
};
