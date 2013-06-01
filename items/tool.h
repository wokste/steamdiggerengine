#pragma once
#include "itemdef.h"
#include <string>
#include "enums.h"

class World;
class BlockType;

class Tool : public ItemDef{
public:
	Tool(const ConfigNode& config);

	virtual double use(Player& owner, ItemStack& item, const Screen& screen);
private:
	const BlockType& mineAt(World& world, Vector2i pos, int layer);
};
