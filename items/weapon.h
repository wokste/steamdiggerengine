#pragma once
#include "itemdef.h"
#include <string>
class ProjectileStats;

class Weapon : public ItemDef{
public:
	Weapon();
	~Weapon();

	ProjectileStats* projectileType;
	virtual bool use(Player& owner, ItemStack& item, Screen& screen);
	virtual void load(ConfigNode& config);
};
