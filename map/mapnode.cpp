/*
The MIT License (MIT)

Copyright (c) 2013, Steven Wokke

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "mapnode.h"
#include "blocktype.h"
#include "../utils/texture.h"
#include "../utils/skybox.h"
#include "../enums.h"
#include "map.h"
#include <iostream>
#include "../game.h"
#include "../attack.h"

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

void MapNode::render(const sf::Color& skyColor, Vector2i pos, int focussedLayer) const{
	sf::Color currentLight = getLight(skyColor);
	for (int layer = 0; layer < Layer::count; ++layer){
		double brightness = (layer == focussedLayer) ? 1.0 : 0.75;
		glColor3ub(currentLight.r * brightness, currentLight.g * brightness, currentLight.b * brightness);
		if (modelId[layer] >= 0){
			glPushMatrix();
			glTranslated(pos.x, pos.y, layer);
			(*GameGlobals::blockDefs)[blockId[layer]].models[modelId[layer]].render();
			glPopMatrix();
		}
	}
}

bool MapNode::damageBlock(int layer, const Attack& attack){
	auto blockMaterial = (*GameGlobals::blockDefs)[blockId[layer]].materialType;
	auto damage = attack.damage;

	HP[layer] -= damage;
	if (HP[layer] <= 0){
		setBlock(0, layer);
		return true;
	}
	return false;
}
