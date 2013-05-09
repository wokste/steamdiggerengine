#pragma once
#include "itemdef.h"
#include <string>
class ProjectileStats;

class Weapon : public ItemDef{
public:
	Weapon(const Game& game, const ConfigNode& config);
	~Weapon();

	ProjectileStats* projectile;
	virtual int use(Player& owner, ItemStack& item, const Screen& screen);
};
