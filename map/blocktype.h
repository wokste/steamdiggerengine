#pragma once
#include "../items/enums.h" // TODO: Move this file
#include "../utils/vertex.h"
#include <SFML/Graphics/Color.hpp>

class ConfigNode;
class Game;

class BlockType{
public:
	BlockCollisionType collisionType;
	BlockMaterialType materialType;
	//int startFrame;
	//int numFrames;
	double timeToMine;
	sf::Color lightColor;
	VertexArray model;

	BlockType(const ConfigNode& config);
	void addDrop(int itemID, float chance = 1);
};
