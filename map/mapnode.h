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
	void setBlock(int blockId, int layer);
	const BlockType& getBlock(int layer) const;
	void render(const sf::Color& skyColor, Vector2i pos) const;
	bool isset(int layer) const{ return blockId[layer] != 0;}
	sf::Color light[2];
	sf::Color getLight(const sf::Color& skyColor) const;
	bool damageBlock(int layer, int damageHigh, int damageLow, int damageTypeSet);

	static constexpr int MAX_LAYERS = 2;
private:
	int modelId[MAX_LAYERS];
	int blockId[MAX_LAYERS];
	int HP[MAX_LAYERS];
};
