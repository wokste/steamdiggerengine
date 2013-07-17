#include "itemdefmanager.h"
#include "item.h"

#include "../utils/confignode.h"
#include "../utils/texture.h"
#include "../map/blocktype.h"
#include "../effects/mapeffect.h"
#include <iostream>

ItemDefManager::ItemDefManager(const std::string& jsonFileName){
	ConfigNode::load(jsonFileName, [&] (ConfigNode& jsonArray){
		jsonArray.forEachNode([&] (ConfigNode& json) {
			insert(std::unique_ptr<ItemType>(new ItemType(json)), json.getString("tag",""));
		});
	});
}

ItemDefManager::~ItemDefManager(){

}

/** inserts item. returns the itemID */
int ItemDefManager::insert(std::unique_ptr<ItemType> type, const std::string tag){
	items.push_back(std::move(type));
	int id = items.size() - 1;
	if(tag != ""){
		tags.insert(make_pair(tag, id));
	}
	return id;
}

/** creates item for building block. returns the itemID */
int ItemDefManager::addBuildingBlock(int blockID, int frameID, const std::string tag){
	auto item = std::unique_ptr<ItemType>(new ItemType(frameID));
	item->onUse.add(std::unique_ptr<Effect>(new BuildEffect(blockID)));
	return insert(std::move(item), tag);
}
