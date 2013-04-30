#pragma once
#include "itemdef.h"
#include <string>
class EntityStats;

class Weapon : public ItemDef{
public:
	Weapon(Game& game, ConfigNode& config);
	~Weapon();

	EntityStats* entitiyType;
	virtual int use(Player& owner, ItemStack& item, Screen& screen);
};
