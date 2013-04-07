#pragma once
#include "itemdef.h"
#include <string>
class ProjectileStats;

class Weapon : public ItemDef{
public:
	Weapon();

	std::string projectileName;
	virtual bool use(Player& owner, ItemStack& item, Screen& screen);
	virtual void load(ConfigNode& config);
};
