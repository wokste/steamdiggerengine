#pragma once
#include "../enums.h"
#include <vector>
#include "../utils/vertex.h"
#include "../utils/drop.h"
#include <SFML/Graphics/Color.hpp>

class ConfigNode;
class World;

class BlockType{
public:
	BlockCollisionType collisionType;
	BlockMaterialType materialType;
	int HP;
	sf::Color lightColor;
	sf::Color blockedLight;
	std::vector<VertexArray> models;

	BlockType(const ConfigNode& config);
	int getModelId() const;

	DropList drops;
};

class BlockTypeManager{
	std::vector<BlockType> blocks;
public:
	BlockTypeManager(std::string jsonFileName);
	~BlockTypeManager();
	BlockType& operator[](int id);
};
