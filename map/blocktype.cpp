#include "blocktype.h"

#include "lightingengine.h"
#include "../items/enums.h"
#include "../utils/confignode.h"

BlockType::BlockType(const ConfigNode& config, std::vector<VertexArray>& modelList){
	collisionType = getBlockCollisionType(config.getString("collision", "Air"));
	materialType = getBlockMaterialType(config.getString("material", "None"));
	HP = config.getDouble("hp", -1);
	lightColor = LightingEngine::makeColor(config.getString("light", "0"));

	const_cast<ConfigNode&>(config).getNode("frame").forEachNode([&] (const ConfigNode& json) {
		auto framesPerSheet = sf::Vector2i(4,4);
		auto id      = json.getInt("id",-1);
		auto display = json.getString("display","cube");
		modelList.push_back(VertexArray(display,id,framesPerSheet));
		modelNums.push_back(modelList.size() - 1);
	});
}

void BlockType::addDrop(int itemID, float chance){
	//TODO: Implement using effects
}

int BlockType::getModelId() const{
	if (modelNums.size() == 0)
		return -1; // Negative means no model ID

	// TODO: Make it possible to have rare variants
	return modelNums[rand() % modelNums.size()];
}
