#include "lightingengine.h"
#include <SFML/Graphics/Color.hpp>
#include "map.h"
#include "mapnode.h"
#include "../items/block.h"
#include "../items/itemdefmanager.h"

// Private functions
namespace LightingEngine{
	bool brightenColor(const sf::Color& appliedFrom, sf::Color& applyTo);
	bool isBrighter(const sf::Color& left, const sf::Color& right);
	sf::Color getBlockedLight(const Block* block);

	void applyLight(Map& map, const Vector2i& pos, const sf::Color& light, int lightType);
	void copyLight(Map& map, const Vector2i& pos1, const Vector2i& pos2);
};

sf::Color LightingEngine::getBlockedLight(const Block* block){
	return block ? sf::Color(100,100,100) : sf::Color(10,10,10);
}

sf::Color LightingEngine::makeColor(const std::string& str){
	long int colorInt = strtol(str.c_str(), nullptr, 16);
	return sf::Color(
		(colorInt >> 16) & 0xff,
		(colorInt >>  8) & 0xff,
		(colorInt >>  0) & 0xff
	);
}

sf::Color operator-(const sf::Color& left, const sf::Color& right){
	return sf::Color(
		left.r > right.r ? left.r - right.r : 0,
		left.g > right.g ? left.g - right.g : 0,
		left.b > right.b ? left.b - right.b : 0,
		left.a > right.a ? left.a - right.a : 0
	);
}

bool LightingEngine::isBrighter(const sf::Color& left, const sf::Color& right){
	return (
		left.r > right.r ||
		left.g > right.g ||
		left.b > right.b
	);
}

bool LightingEngine::brightenColor(const sf::Color& appliedFrom, sf::Color& applyTo){
	bool success = false;
	if (applyTo.r < appliedFrom.r){
		success = true;
		applyTo.r = appliedFrom.r;
	}
	if (applyTo.g < appliedFrom.g){
		success = true;
		applyTo.g = appliedFrom.g;
	}
	if (applyTo.b < appliedFrom.b){
		success = true;
		applyTo.b = appliedFrom.b;
	}
	return success;
}

void LightingEngine::applyLight(Map& map, const Vector2i& pos, const sf::Color& light, int lightType) {
	MapNode* node = map.getMapNode(pos.x, pos.y);
	if (node == nullptr)
		return;

	Block* block = node->getBlock(map.itemDefs,0);

	if (!brightenColor(light, node->light[lightType]))
		return;

	sf::Color newLight = light - getBlockedLight(block);
	// TODO: this should be improved

	node->light[lightType] = light;

	// == recursive part ==
	applyLight(map, pos + Vector2i( 1, 0), newLight, lightType);
	applyLight(map, pos + Vector2i(-1, 0), newLight, lightType);
	applyLight(map, pos + Vector2i( 0, 1), newLight, lightType);
	applyLight(map, pos + Vector2i( 0,-1), newLight, lightType);
}

void LightingEngine::copyLight(Map& map, const Vector2i& pos1, const Vector2i& pos2) {
	MapNode* node = map.getMapNode(pos1.x, pos1.y);
	if (node == nullptr)
		return;

	Block* block = node->getBlock(map.itemDefs, 0);
	sf::Color blockedLight = getBlockedLight(block);
	// TODO: this should be improved

	applyLight(map, pos2, node->light[0] - blockedLight, 0);
	applyLight(map, pos2, node->light[1] - blockedLight, 1);
}

void LightingEngine::recalcAreaAround(Map& map, const Vector2i& pos) {
	int lightRadius = 255 / 10 + 1;
	for (int x = pos.x - lightRadius; x <= pos.x + lightRadius; x++){
		for (int y = pos.y - lightRadius; y <= pos.y + lightRadius; y++){
			MapNode* node = map.getMapNode(x, y);
			if (node == nullptr)
				continue;

			node->light[0] = sf::Color::Black;
			node->light[1] = sf::Color::Black;
		}
	}

	// == Re-add lights in area
	for (int x = pos.x - lightRadius; x <= pos.x + lightRadius; x++){
		for (int y = pos.y - lightRadius; y <= pos.y + lightRadius; y++){
			MapNode* node = map.getMapNode(x, y);
			if (node == nullptr)
				continue;

			Block* frontBlock = node->getBlock(map.itemDefs, 0);
			if (frontBlock != nullptr && frontBlock->lightColor != sf::Color::Black){
				applyLight(map, Vector2i(x,y), frontBlock->lightColor, 0);
			}
		}
	}

	// Add lighting from top and bottom borders
	for (int x = pos.x - lightRadius; x <= pos.x + lightRadius; x++){
		// top
		copyLight(map, Vector2i(x, pos.y - lightRadius - 1), Vector2i(x, pos.y - lightRadius));
		copyLight(map, Vector2i(x, pos.y + lightRadius + 1), Vector2i(x, pos.y + lightRadius));
	}

	// Add lighting from left and right borders
	for (int y = pos.y - lightRadius; y <= pos.y + lightRadius; y++){
		copyLight(map, Vector2i(pos.x - lightRadius - 1, y), Vector2i(pos.x - lightRadius, y));
		copyLight(map, Vector2i(pos.x + lightRadius + 1, y), Vector2i(pos.x + lightRadius, y));
	}
}