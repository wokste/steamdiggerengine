#include "enums.h"

BlockCollisionType getBlockCollisionType(std::string name){
	if (name == "Air")
		return BlockCollisionType::Air;
	if (name == "Solid")
		return BlockCollisionType::Solid;
	if (name == "Ladder")
		return BlockCollisionType::Ladder;
	return BlockCollisionType::Air;
}

BlockMaterialType getBlockMaterialType(std::string name){
	if (name == "None")
		return BlockMaterialType::None;
	if (name == "Wood")
		return BlockMaterialType::Wood;
	if (name == "Dirt")
		return BlockMaterialType::Dirt;
	if (name == "Stone")
		return BlockMaterialType::Stone;
	if (name == "Plant")
		return BlockMaterialType::Plant;
	return BlockMaterialType::None;
}

BlockFrameType getBlockFrameType(std::string name){
	if (name == "None")
		return BlockFrameType::None;
	if (name == "X")
		return BlockFrameType::X;
	if (name == "Y")
		return BlockFrameType::Y;
	if (name == "XY")
		return BlockFrameType::XY;
	return BlockFrameType::None;
}
