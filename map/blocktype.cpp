#include "blocktype.h"

#include "lightingengine.h"
#include "../items/enums.h"
#include "../utils/confignode.h"

BlockType::BlockType(const ConfigNode& config){
	collisionType = getBlockCollisionType(config.getString("collision", "Air"));
	materialType = getBlockMaterialType(config.getString("material", "None"));
	auto startFrame = config.getInt("frame-start", -1);
	auto numFrames = config.getInt("frame-count", 1);
	auto timeToMine = config.getDouble("time-to-mine", -1);
	lightColor = LightingEngine::makeColor(config.getString("light", "0"));
	auto sheet = sf::Vector2i(4,4);
	model.createCube(sf::Vector3f(0,0,0),startFrame,sheet);
}

void BlockType::addDrop(int itemID, float chance){
	//TODO: Implement using effects
}
