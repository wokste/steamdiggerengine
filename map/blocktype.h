#pragma once
#include "../items/enums.h" // TODO: Move this file
#include <SFML/Graphics/Color.hpp>

class ConfigNode;

class BlockType{
public:
	BlockCollisionType collisionType;
	BlockMaterialType materialType;
	int startFrame;
	int numFrames;
	double timeToMine;
	sf::Color lightColor;

	BlockType(const ConfigNode& config);

	// TODO: Delete this
	BlockType(int nFrame) {startFrame = nFrame; numFrames = 1; timeToMine = 0.5; lightColor = sf::Color::Black; collisionType = (nFrame == -1) ? BlockCollisionType::Air : BlockCollisionType::Solid;}
};
