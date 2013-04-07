#include "enums.h"

BlockCollisionType getBlockCollisionType(std::string name){
	if (name == "air")
		return BlockCollisionType::Air;
	if (name == "solid")
		return BlockCollisionType::Solid;
	if (name == "ladder")
		return BlockCollisionType::Ladder;
	return BlockCollisionType::Air;
}

BlockMaterialType getBlockMaterialType(std::string name){
	if (name == "none")
		return BlockMaterialType::None;
	if (name == "wood")
		return BlockMaterialType::Wood;
	if (name == "dirt")
		return BlockMaterialType::Dirt;
	if (name == "stone")
		return BlockMaterialType::Stone;
	if (name == "plant")
		return BlockMaterialType::Plant;
	return BlockMaterialType::None;
}
