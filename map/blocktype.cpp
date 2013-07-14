#include "blocktype.h"

#include "lightingengine.h"
#include "../items/enums.h"
#include "../utils/confignode.h"

BlockType::BlockType(const ConfigNode& config){
	collisionType = getBlockCollisionType(config.getString("collision", "Air"));
	materialType = getBlockMaterialType(config.getString("material", "None"));
	startFrame = config.getInt("frame-start", -1);
	numFrames = config.getInt("frame-count", 1);
	timeToMine = config.getDouble("time-to-mine", -1);
	lightColor = LightingEngine::makeColor(config.getString("light", "0"));
}

void BlockType::addDrop(int itemID, float chance){
	//TODO: Implement using effects
}
