#include "mapnode.h"
#include "blocktype.h"
#include "../utils/texture.h"
#include "../utils/skybox.h"
#include "../enums.h"
#include "map.h"
#include <iostream>
#include "../game.h"

MapNode::MapNode(){
	for (int i = 0; i < MAX_LAYERS; ++i){
		modelId[i] = -1;
		blockId[i] = 0;
	}
	light[0] = sf::Color::Black;
	light[1] = sf::Color::Black;
}

void MapNode::setBlock(int newBlockId, int layer){
	const BlockType& type = (*GameGlobals::blockDefs)[newBlockId];
	modelId[layer] = type.getModelId();
	blockId[layer] = newBlockId;
}

const BlockType& MapNode::getBlock(int layer) const{
	return (*GameGlobals::blockDefs)[blockId[layer]];
}

sf::Color MapNode::getLight(const sf::Color& skyColor) const{
	return light[LightType::placed] + light[LightType::sky] * skyColor;
}

void MapNode::render(const sf::Color& skyColor, Vector2i pos) const{
	sf::Color currentLight = getLight(skyColor);
	glColor3ub(currentLight.r, currentLight.g, currentLight.b);
	if (modelId[Layer::front] >= 0){
		glPushMatrix();
		glTranslated(pos.x, pos.y, Layer::front);
		(*GameGlobals::blockDefs)[blockId[Layer::front]].models[modelId[Layer::front]].render();
		glPopMatrix();
	}

	glColor3ub(currentLight.r * 0.8, currentLight.g * 0.8, currentLight.b * 0.8);
	if (modelId[Layer::back] >= 0){
		glPushMatrix();
		glTranslated(pos.x, pos.y, Layer::back);
		(*GameGlobals::blockDefs)[blockId[Layer::back]].models[modelId[Layer::back]].render();
		glPopMatrix();
	}
}
