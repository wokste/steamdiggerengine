#include "blocktype.h"

#include "lightingengine.h"
#include "../enums.h"
#include "../utils/confignode.h"
#include "../game.h"
#include "../items/itemdefmanager.h"

BlockType::BlockType(const ConfigNode& config){
	collisionType = getBlockCollisionType(config.getString("collision", "Air"));
	materialType = getBlockMaterialType(config.getString("material", "None"));
	HP = config.getDouble("hp", -1);
	lightColor = LightingEngine::makeColor(config.getString("light", "0"));
	unsigned char blockedLightByte = config.getInt("blocked-light", 100);
	blockedLight = sf::Color(blockedLightByte,blockedLightByte,blockedLightByte);

	const_cast<ConfigNode&>(config).getNode("frame").forEachNode([&] (const ConfigNode& json) {
		auto framesPerSheet = sf::Vector2i(8,8);
		auto id      = json.getInt("id",-1);
		auto display = json.getString("display","cube");
		models.push_back(VertexArray(display,id,framesPerSheet));
	});
}

int BlockType::getModelId() const{
	// TODO: Make it possible to have rare variants
	if (models.empty())
		return -1;
	return rand() % models.size();
}

BlockTypeManager::BlockTypeManager(std::string jsonFileName){
	ConfigNode::load(jsonFileName, [&] (ConfigNode& jsonArray){
		jsonArray.forEachNode([&] (ConfigNode& json) {
			BlockType block = BlockType(json);
			std::string dropTag = json.getString("drop","_default");
			if (dropTag == "_default"){
				int blockID = blocks.size();
				int iconFrame = json.getNode("frame")[0].getInt("id");
				int dropID = GameGlobals::itemDefs->addBuildingBlock(blockID, iconFrame, json.getString("tag",""));
				block.drops.emplace_back(dropID);
			} else if (dropTag != ""){
				// TODO: Exception handling
				// TODO: Lazy evaluation of dropTag
				block.drops.emplace_back(dropTag);
			}
			blocks.push_back(block);
		});
	});
}

BlockTypeManager::~BlockTypeManager(){
}

BlockType& BlockTypeManager::operator[](int id){
	return blocks[id];
}
