#include "itemdefmanager.h"
#include "block.h"
#include "gun.h"
#include "armour.h"
#include "tool.h"

#include "../utils/confignode.h"
#include "../utils/assert.h"
#include "../utils/texture.h"
#include <iostream>

ItemDefManager::ItemDefManager(const Game& game, const std::string& configFileName){
	int id = 0;

	ConfigNode::load(configFileName, [&] (ConfigNode& configArray){
		configArray.forEachNode([&] (ConfigNode& config) {
			ItemDef* stat = nullptr;
			std::string className = config.getString("class");

			if (className == "block")
				stat = new Block(config);
			if (className == "tool")
				stat = new Tool(config);
			if (className == "gun")
				stat = new Gun(game, config);
			if (className == "armour")
				stat = new Armour(config);

			ASSERT(stat, configFileName, "Unknown class " + className);
			if (stat != nullptr){
				stat->ID = id;
				id++;
				itemDefs.push_back(stat);
			}
		});
	});
}

ItemDefManager::~ItemDefManager(){
}
