#pragma once
#include "itemdef.h"
#include <string>
#include "enums.h"

class World;

class Tool : public ItemDef{
public:
	Tool(ConfigNode& config);

	virtual bool use(Player& owner, ItemStack& item, Screen& screen);
private:
	int mineAt(World* world, Vector2i pos, int layer);
};
