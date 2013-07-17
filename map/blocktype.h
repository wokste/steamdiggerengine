#pragma once
#include "../items/enums.h" // TODO: Move this file
#include <vector>
#include "../utils/vertex.h"
#include <SFML/Graphics/Color.hpp>

class ConfigNode;

class BlockType{
public:
	BlockCollisionType collisionType;
	BlockMaterialType materialType;
	int HP;
	sf::Color lightColor;

	BlockType(const ConfigNode& config, std::vector<VertexArray>& modelList);
	void addDrop(int itemID, float chance = 1);
	std::vector<int> modelNums;
	int getModelId() const;
};
