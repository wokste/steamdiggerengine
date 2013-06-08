#include "mapnode.h"
#include "blocktype.h"
#include "../utils/texture.h"
#include "../utils/skybox.h"
#include "../enums.h"
#include "map.h"
#include <iostream>

MapNode::MapNode(){
	light[Layer::front] = sf::Color::Black;
	frame[Layer::front] = -1;
	blockId[Layer::front] = 0;
	light[Layer::back] = sf::Color::Black;
	frame[Layer::back] = -1;
	blockId[Layer::back] = 0;
}

void MapNode::setBlock(const Map& map, int newBlockId, int layer){
	const BlockType& type = map.blockDefs[newBlockId];
	frame[layer] = type.startFrame + rand() % type.numFrames;
	blockId[layer] = newBlockId;
}

const BlockType& MapNode::getBlock(const Map& map, int layer) const{
	return map.blockDefs[blockId[layer]];
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
