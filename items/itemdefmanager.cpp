#include "itemdefmanager.h"
#include "item.h"

#include "../utils/confignode.h"
#include "../utils/assert.h"
#include "../utils/texture.h"
#include <iostream>

ItemDefManager::ItemDefManager(const Game& game, const std::string& configFileName){
	ConfigNode::load(configFileName, [&] (ConfigNode& configArray){
		configArray.forEachNode([&] (ConfigNode& config) {
			items.emplace_back(new ItemType(config));
		});
	});
}

ItemDefManager::~ItemDefManager(){
}
