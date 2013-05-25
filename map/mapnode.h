#pragma once;
#include "../utils/vector2.h"
#include <SFML/Graphics/Color.hpp>

class Block;
class ItemDefManager;
class Texture;
class Skybox;

class MapNode{
public:
	MapNode();
	void setBlock(Block* block, int layer);
	Block* getBlock(ItemDefManager* itemDefs, int layer);
	void render(const sf::Color& skyColor, Texture& tileset, Vector2i pos) const;
	bool isset(int layer){ return blockId[layer] != -1;}
	sf::Color light[2];
	sf::Color getLight(const sf::Color& skyColor) const;
private:
	static constexpr int MAX_LAYERS = 2;
	int frame[MAX_LAYERS];
	int blockId[MAX_LAYERS];
};
