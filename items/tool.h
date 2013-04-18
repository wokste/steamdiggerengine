#pragma once
#include "itemdef.h"
#include <string>
#include "enums.h"

class World;

class Tool : public ItemDef{
public:
	Tool();

	virtual bool use(Player& owner, ItemStack& item, Screen& screen);
	virtual void load(ConfigNode& config);
private:
	int mineAt(World* world, Vector2i pos, int layer);
};
