#pragma once
#include "itemdef.h"
#include <string>
class EntityStats;

class Weapon : public ItemDef{
public:
	Weapon(const Game& game, const ConfigNode& config);
	~Weapon();

	EntityStats* entitiyType;
	virtual int use(Player& owner, ItemStack& item, const Screen& screen);
};
