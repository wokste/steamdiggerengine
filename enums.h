#pragma once

enum class BlockCollisionType
{
	Air, Solid, Ladder
};
BlockCollisionType getBlockCollisionType(std::string);

enum class BlockMaterialType{
	None = 0, Wood = 1, Dirt = 2, Stone = 4, Plant = 8
};
BlockMaterialType getBlockMaterialType(std::string);

namespace Layer{
	constexpr int front = 0;
	constexpr int back = 1;
}

namespace LightType{
	constexpr int placed = 0;
	constexpr int sky = 1;
}
