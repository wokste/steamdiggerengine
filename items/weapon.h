#pragma once
#include "itemdef.h"
#include <string>
class EntityStats;

class Weapon : public ItemDef{
public:
	Weapon(GameSettings& gameSettings, ConfigNode& config);
	~Weapon();

	EntityStats* entitiyType;
	virtual int use(Player& owner, ItemStack& item, Screen& screen);
};
