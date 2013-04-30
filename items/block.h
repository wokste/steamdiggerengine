#pragma once
#include "itemdef.h"
#include <string>
#include "enums.h"

class World;

class Block : public ItemDef{
public:
	BlockCollisionType collisionType;
	BlockMaterialType materialType;
	int startFrame;
	int numFrames;
	double timeToMine;

	Block(const ConfigNode& config);
	virtual int use(Player& owner, ItemStack& item, const Screen& screen);

	virtual Block* asBlock(){return this;}
private:
	bool placeAt(World* world, Vector2i pos, int layer);
};
