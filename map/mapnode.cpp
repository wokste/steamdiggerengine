#include "mapnode.h"
#include "blocktype.h"
#include "../utils/texture.h"
#include "../utils/skybox.h"
#include "../enums.h"
#include "map.h"
#include <iostream>
#include "../game.h"

MapNode::MapNode(){
	for (int i = 0; i < Layer::count; ++i){
		modelId[i] = -1;
		blockId[i] = 0;
		HP[i] = 0;
	}
	for (int i = 0; i < LightType::count; ++i)
		light[i] = sf::Color::Black;
}

void MapNode::setBlock(int newBlockId, int layer){
	const BlockType& type = (*GameGlobals::blockDefs)[newBlockId];
	modelId[layer] = type.getModelId();
	blockId[layer] = newBlockId;
	HP[layer] = type.HP;
}

const BlockType& MapNode::getBlock(int layer) const{
	return (*GameGlobals::blockDefs)[blockId[layer]];
}

sf::Color MapNode::getLight(const sf::Color& skyColor) const{
	return light[LightType::placed] + light[LightType::sky] * skyColor;
}

void MapNode::render(const sf::Color& skyColor, Vector2i pos) const{
	sf::Color currentLight = getLight(skyColor);
	for (int layer = 0; layer < Layer::count; ++layer){
		glColor3ub(currentLight.r, currentLight.g, currentLight.b);
		if (modelId[layer] >= 0){
			glPushMatrix();
			glTranslated(pos.x, pos.y, layer);
			(*GameGlobals::blockDefs)[blockId[layer]].models[modelId[layer]].render();
			glPopMatrix();
		}
	}
}

bool MapNode::damageBlock(int layer, int damageHigh, int damageLow, int preferredMaterial){
	auto blockMaterial = (*GameGlobals::blockDefs)[blockId[layer]].materialType;
	auto damage = ((preferredMaterial & (int)blockMaterial) != 0) ? damageHigh : damageLow;

	HP[layer] -= damage;
	if (HP[layer] <= 0){
		setBlock(0, layer);
		return true;
	}
	return false;
}
