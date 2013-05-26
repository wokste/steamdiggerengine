#pragma once
#include "itemdef.h"
#include <string>
#include "enums.h"

class World;

class Tool : public ItemDef{
public:
	Tool(const ConfigNode& config);

	virtual double use(Player& owner, ItemStack& item, const Screen& screen);
private:
	Block* mineAt(World& world, Vector2i pos, int layer);
};
