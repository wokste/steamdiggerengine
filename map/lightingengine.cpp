#include "lightingengine.h"
#include <SFML/Graphics/Color.hpp>
#include "map.h"
#include "mapnode.h"
#include "blocktype.h"
#include "../enums.h"

// Private functions
namespace LightingEngine{
	bool brightenColor(const sf::Color& appliedFrom, sf::Color& applyTo);
	bool isBrighter(const sf::Color& left, const sf::Color& right);
	sf::Color getBlockedLight(const MapNode& node);

	void applyLight(Map& map, const Vector2i& pos, const sf::Color& light, int lightType);
	void copyLight(Map& map, const Vector2i& pos1, const Vector2i& pos2);
	sf::Color airColor = sf::Color(192,192,192);
};

/// Gives the value that is blocked by the given block. Block may be null for the sky.
sf::Color LightingEngine::getBlockedLight(const MapNode& node){
	return (node.isset(Layer::front)) ? sf::Color(100,100,100) : sf::Color(10,10,10);
}

/// precondition: str is formatted in a 6 character hexdecimal color code (known from css)
/// E.g. red: "ff0000", green "00ff00", blue "0000ff"
sf::Color LightingEngine::makeColor(const std::string& str){
	long int colorInt = strtol(str.c_str(), nullptr, 16);
	return sf::Color(
		(colorInt >> 16) & 0xff,
		(colorInt >>  8) & 0xff,
		(colorInt >>  0) & 0xff
	);
}

sf::Color operator*(const sf::Color& left, const double right){
	return sf::Color(
		left.r * right,
		left.g * right,
		left.b * right,
		left.a
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

	if (!brightenColor(light, node->light[lightType]))
		return;

	node->light[lightType] = light;

	sf::Color newLight = light - getBlockedLight(*node);

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

	sf::Color blockedLight = getBlockedLight(*node);

	applyLight(map, pos2, node->light[LightType::placed] - blockedLight, LightType::placed);
	applyLight(map, pos2, node->light[LightType::sky] - blockedLight, LightType::sky);
}

void LightingEngine::recalcAreaAround(Map& map, const Vector2i& pos) {
	int lightRadius = 255 / 20 + 1;
	recalcArea(map, Vector2i(pos.x - lightRadius, pos.y - lightRadius),Vector2i(pos.x + lightRadius, pos.y + lightRadius));
}

void LightingEngine::recalcArea(Map& map, const Vector2i& pos1, const Vector2i& pos2) {
	for (int x = pos1.x; x <= pos2.x; x++){
		for (int y = pos1.y; y <= pos2.y; y++){
			MapNode* node = map.getMapNode(x, y);
			if (node == nullptr)
				continue;

			node->light[LightType::placed] = sf::Color::Black;
			node->light[LightType::sky] = sf::Color::Black;
		}
	}

	// == Re-add lights in area
	for (int x = pos1.x; x <= pos2.x; x++){
		for (int y = pos1.y; y <= pos2.y; y++){
			MapNode* node = map.getMapNode(x, y);
			if (node == nullptr)
				continue;

			const BlockType& frontBlock = node->getBlock(Layer::front);
			if (node->isset(Layer::front) && frontBlock.lightColor != sf::Color::Black){
				applyLight(map, Vector2i(x,y), frontBlock.lightColor, LightType::placed);
			}

			if (!node->isset(Layer::back)){
				applyLight(map, Vector2i(x,y), airColor, LightType::sky);
			}
		}
	}

	// Add lighting from top and bottom borders
	for (int x = pos1.x; x <= pos2.x; x++){
		// top
		copyLight(map, Vector2i(x, pos1.y - 1), Vector2i(x, pos1.y));
		copyLight(map, Vector2i(x, pos2.y + 1), Vector2i(x, pos2.y));
	}

	// Add lighting from left and right borders
	for (int y = pos1.y; y <= pos2.y; y++){
		copyLight(map, Vector2i(pos1.x - 1, y), Vector2i(pos1.x, y));
		copyLight(map, Vector2i(pos2.x + 1, y), Vector2i(pos2.x, y));
	}
}
