#pragma once
#include "itemdef.h"

class World;

class Block : public ItemDef{
public:
	int blockTypeID;
	Block(const ConfigNode& config);
	virtual double use(Player& owner, ItemStack& item, const Screen& screen);

	virtual Block* asBlock(){return this;}
private:
	bool placeAt(World& world, Vector2i pos, int layer);
};
