#pragma once
#include <string>

enum class BlockCollisionType
{
	Air, Solid, Ladder
};
BlockCollisionType getBlockCollisionType(std::string);

enum class BlockMaterialType{
	None, Wood, Dirt, Stone, Plant
};
BlockMaterialType getBlockMaterialType(std::string);
