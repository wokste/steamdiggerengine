#pragma once
#include "../utils/vector2.h"
namespace sf{
	class Color;
};
class Map;

namespace LightingEngine{
	sf::Color makeColor(const std::string& str);
	void recalcAreaAround(Map& map, const Vector2i& pos);
};
