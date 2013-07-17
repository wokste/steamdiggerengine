#include "mapnode.h"
#include "blocktype.h"
#include "../utils/texture.h"
#include "../utils/skybox.h"
#include "../enums.h"
#include "map.h"
#include <iostream>

MapNode::MapNode(){
	for (int i = 0; i < MAX_LAYERS; ++i){
		modelId[i] = -1;
		blockId[i] = 0;
	}
	light[0] = sf::Color::Black;
	light[1] = sf::Color::Black;
}

void MapNode::setBlock(const Map& map, int newBlockId, int layer){
	const BlockType& type = map.blockDefs[newBlockId];
	modelId[layer] = type.getModelId();
	blockId[layer] = newBlockId;
}

const BlockType& MapNode::getBlock(const Map& map, int layer) const{
	return map.blockDefs[blockId[layer]];
}

sf::Color MapNode::getLight(const sf::Color& skyColor) const{
	return light[LightType::placed] + light[LightType::sky] * skyColor;
}

void MapNode::render(const Map& map, const sf::Color& skyColor, Texture& tileSet, Vector2i pos) const{
	sf::Color currentLight = getLight(skyColor);
	glColor3ub(currentLight.r, currentLight.g, currentLight.b);
	if (modelId[Layer::front] >= 0){
		glPushMatrix();
		glTranslated(pos.x, pos.y, Layer::front);
		map.models[modelId[Layer::front]].render();
		glPopMatrix();
	}

	glColor3ub(currentLight.r * 0.8, currentLight.g * 0.8, currentLight.b * 0.8);
	if (modelId[Layer::back] >= 0){
		glPushMatrix();
		glTranslated(pos.x, pos.y, Layer::back);
		map.models[modelId[Layer::back]].render();
		glPopMatrix();
	}
}
