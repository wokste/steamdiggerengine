#pragma once
#include "../enums.h"
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
	std::vector<VertexArray> models;

	BlockType(const ConfigNode& config);
	void addDrop(int itemID, float chance = 1);
	int getModelId() const;
};

class BlockTypeManager{
	std::vector<BlockType> blocks;
public:
	BlockTypeManager(std::string jsonFileName);
	~BlockTypeManager();
	BlockType& operator[](int id);
};
