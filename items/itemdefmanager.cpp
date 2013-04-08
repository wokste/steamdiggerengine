#include "itemdefmanager.h"
#include "block.h"
#include "../utils/confignode.h"
#include "../utils/assert.h"
#include <iostream>

ItemDefManager::ItemDefManager(){
	std::string fileName = "blocks.json";
	std::cout << "loading " << fileName << "\n";
	int id = 0;

	ConfigNode::load(fileName, [&] (ConfigNode& configArray){
		configArray.forEachNode([&] (ConfigNode& config) {
			ItemDef * stat = nullptr;
			std::string className = config.getString("class");

#define OPTION(str,class) if (className == str) {stat = new class ();}
			OPTION("block",Block)
#undef OPTION

			ASSERT(stat, fileName, "Unknown class " + className);
			if (stat != nullptr){
				stat->ID = id;
				id++;
				stat->load(config);
				itemDefs.push_back(stat);
			}
		});
	});
}
ItemDef* ItemDefManager::getItemDef(int id){
	return itemDefs[id];
}
