#pragma once;
#include "../utils/vector2.h"
#include <SFML/Graphics/Color.hpp>

class Block;
class ItemDefManager;
class Texture;
class Skybox;
class BlockType;
class Map;

class MapNode{
public:
	MapNode();
	void setBlock(const Map& map, int blockId, int layer);
	const BlockType& getBlock(const Map& map, int layer) const;
	void render(const Map& map, const sf::Color& skyColor, Texture& tileset, Vector2i pos) const;
	bool isset(int layer) const{ return blockId[layer] != 0;}
	sf::Color light[2];
	sf::Color getLight(const sf::Color& skyColor) const;
private:
	static constexpr int MAX_LAYERS = 2;
	int frame[MAX_LAYERS];
	int blockId[MAX_LAYERS];
};
