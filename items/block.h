#pragma once
#include "itemdef.h"
#include <string>
#include "enums.h"

class Block : public ItemDef{
public:
	BlockCollisionType collisionType;
	BlockMaterialType materialType;
	BlockFrameType frameType;
	int startFrame;
	double timeToMine;

	Block(const std::string& name);
	virtual bool use(Player& owner, ItemStack& item, Vector2i mousePos);
	virtual void load(ConfigNode& config);
};
