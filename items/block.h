#pragma once
#include "itemdef.h"
#include <string>

enum class BlockCollisionType{
	Air, Solid, Ladder
};

enum class BlockMaterialType{
	None=0, Wood=1, Stone=2, Plant=4
};

enum class BlockFrameType{
	None, X, Y, XY, Random4
};

class Block : public ItemDef{
public:
	BlockCollisionType collisionType;
	BlockMaterialType materialType;
	BlockFrameType frameType;
	int startFrame;
	double timeToMine;

	Block(const std::string& name);
	virtual bool use(Player& owner, ItemStack& item, Vector2i mousePos);
};
