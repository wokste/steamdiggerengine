#include "itemdefmanager.h"
#include "item.h"

#include "../utils/confignode.h"
#include "../utils/assert.h"
#include "../utils/texture.h"
#include <iostream>

ItemDefManager::ItemDefManager(const Game& game, const std::string& jsonFileName){
	ConfigNode::load(jsonFileName, [&] (ConfigNode& jsonArray){
		jsonArray.forEachNode([&] (ConfigNode& json) {
			insert(std::unique_ptr<ItemType>(new ItemType(json)), json.getString("tag",""));
		});
	});
}

ItemDefManager::~ItemDefManager(){

}

int ItemDefManager::insert(std::unique_ptr<ItemType> type, const std::string tag){
	items.push_back(std::move(type));
	if(tag != ""){
		tags.insert(make_pair(tag, (items.size() - 1)));
	}
}
