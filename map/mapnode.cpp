#include "mapnode.h"
#include "../items/block.h"
#include "../utils/texture.h"
#include "../items/itemdefmanager.h"

MapNode::MapNode(){
	frame[0] = -1;
	blockId[0] = -1;
	frame[1] = -1;
	blockId[1] = -1;
	light[0] = 0;
	light[1] = 0;
}

void MapNode::setBlock(Block* block, int layer){
	if (block == nullptr){
		frame[layer] = -1;
		blockId[layer] = -1;
	} else {
		frame[layer] = block->startFrame + rand() % block->numFrames;
		blockId[layer] = block->ID;
	}
	// TODO: Set lighting
}

Block* MapNode::getBlock(ItemDefManager* itemDefs, int layer){
	if (blockId[layer] == -1)
		return nullptr;
	return (*itemDefs)[blockId[layer]]->asBlock();
}

void MapNode::render(Texture& tileSet, Vector2i pos){
	Color currentLight = light[0];
	if (frame[0] != -1){
		Vector3i pos3(pos.x, pos.y, 0);
		tileSet.drawBlock(pos3, frame[0], currentLight);
	}

	if (frame[1] != -1){
		Vector3i pos3(pos.x, pos.y, 1);
		tileSet.drawBlock(pos3, frame[1], currentLight * 0.6);
	}
}
