#include "itemdefmanager.h"
#include "block.h"

ItemDefManager::ItemDefManager(){
	Block* air = new Block("Air");
	air->collisionType = BlockCollisionType::Air;
	air->materialType = BlockMaterialType::None;
	air->frameType = BlockFrameType::None;
	air->startFrame = 42;
	air->timeToMine = -1;
	itemDefs.push_back(air);

	Block* dirt = new Block("Dirt");
	dirt->collisionType = BlockCollisionType::Solid;
	dirt->materialType = BlockMaterialType::Stone;
	dirt->frameType = BlockFrameType::XY;
	dirt->startFrame = 0;
	dirt->timeToMine = 0.7;
	itemDefs.push_back(dirt);

	Block* stone = new Block("Stone");
	stone->collisionType = BlockCollisionType::Solid;
	stone->materialType = BlockMaterialType::Stone;
	stone->frameType = BlockFrameType::XY;
	stone->startFrame = 16;
	stone->timeToMine = 2.0;
	itemDefs.push_back(stone);
}
ItemDef* ItemDefManager::getItemDef(int id){
	return itemDefs[id];
}
