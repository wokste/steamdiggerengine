#pragma once
#include "../utils/vector2.h"
namespace sf{
	class Color;
};
class Map;

namespace LightingEngine{
	sf::Color makeColor(const std::string& str);
	void recalcAreaAround(Map& map, const Vector2i& pos);
	void recalcArea(Map& map, const Vector2i& pos1, const Vector2i& pos2);
};

sf::Color operator*(const sf::Color& left, const double right);
sf::Color operator-(const sf::Color& left, const sf::Color& right);
