#include "mapnode.h"
#include "../items/block.h"
#include "../utils/texture.h"
#include "../items/itemdefmanager.h"

MapNode::MapNode(){
	frame[0] = -1;
	blockId[0] = -1;
	frame[1] = -1;
	blockId[1] = -1;
	light[0] = sf::Color::Black;
	light[1] = sf::Color::Black;
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

sf::Color MapNode::getLight(){
	return light[0] + light[1] * sf::Color(32,32,64);
}

void MapNode::render(Texture& tileSet, Vector2i pos){
	sf::Color currentLight = getLight();
	glColor3ub(currentLight.r, currentLight.g, currentLight.b);
	if (frame[0] != -1){
		Vector3i pos3(pos.x, pos.y, 0);
		tileSet.drawBlock(pos3, frame[0]);
	}

	glColor3ub(currentLight.r * 0.8, currentLight.g * 0.8, currentLight.b * 0.8);
	if (frame[1] != -1){
		Vector3i pos3(pos.x, pos.y, 1);
		tileSet.drawBlock(pos3, frame[1]);
	}
}
