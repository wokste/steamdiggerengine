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

	Block();
	virtual bool use(Player& owner, ItemStack& item, Screen& screen);
	virtual void load(ConfigNode& config);

	virtual Block* asBlock(){return this;}
private:
	bool placeAt(World* world, Vector2i pos, int layer);
};
