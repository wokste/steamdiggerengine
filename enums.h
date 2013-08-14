#pragma once
#include <string>

enum class BlockCollisionType
{
	Air, Solid, Ladder
};
BlockCollisionType getBlockCollisionType(std::string str);

enum class BlockMaterialType{
	None = 0, Wood = 1, Dirt = 2, Stone = 4, Plant = 8
};
BlockMaterialType getBlockMaterialType(std::string str);

namespace Layer{
	constexpr int front = 0;
	constexpr int back = 1;
	constexpr int count = 2;
}

namespace LightType{
	constexpr int placed = 0;
	constexpr int sky = 1;
	constexpr int count = 2;
}
