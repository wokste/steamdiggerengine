#pragma once
#include "itemdef.h"
#include <string>
#include "enums.h"
#include <SFML/Graphics/Color.hpp>

class World;

class Block : public ItemDef{
public:
	BlockCollisionType collisionType;
	BlockMaterialType materialType;
	int startFrame;
	int numFrames;
	double timeToMine;
	sf::Color lightColor;
	Block(const ConfigNode& config);
	virtual double use(Player& owner, ItemStack& item, const Screen& screen);

	virtual Block* asBlock(){return this;}
private:
	bool placeAt(World* world, Vector2i pos, int layer);
};
