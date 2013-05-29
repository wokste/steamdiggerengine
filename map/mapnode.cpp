#include "mapnode.h"
#include "../items/block.h"
#include "../utils/texture.h"
#include "../items/itemdefmanager.h"
#include "../utils/skybox.h"
#include "../enums.h"
#include <iostream>

MapNode::MapNode(){
	light[Layer::front] = sf::Color::Black;
	frame[Layer::front] = -1;
	blockId[Layer::front] = -1;
	light[Layer::back] = sf::Color::Black;
	frame[Layer::back] = -1;
	blockId[Layer::back] = -1;
}

void MapNode::setBlock(Block* block, int layer){
	if (block == nullptr){
		frame[layer] = -1;
		blockId[layer] = -1;
	} else {
		frame[layer] = block->startFrame + rand() % block->numFrames;
		blockId[layer] = block->ID;
	}
}

Block* MapNode::getBlock(ItemDefManager& itemDefs, int layer){
	if (blockId[layer] == -1)
		return nullptr;
	return itemDefs[blockId[layer]]->asBlock();
}

sf::Color MapNode::getLight(const sf::Color& skyColor) const{
	return light[LightType::placed] + light[LightType::sky] * skyColor;
}

void MapNode::render(const sf::Color& skyColor, Texture& tileSet, Vector2i pos) const{
	sf::Color currentLight = getLight(skyColor);
	glColor3ub(currentLight.r, currentLight.g, currentLight.b);
	if (frame[Layer::front] != -1){
		Vector3i pos3(pos.x, pos.y, Layer::front);
		tileSet.drawBlock(pos3, frame[Layer::front]);
	}

	glColor3ub(currentLight.r * 0.8, currentLight.g * 0.8, currentLight.b * 0.8);
	if (frame[Layer::back] != -1){
		Vector3i pos3(pos.x, pos.y, Layer::back);
		tileSet.drawBlock(pos3, frame[Layer::back]);
	}
}
