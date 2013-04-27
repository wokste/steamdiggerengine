#include "itemdefmanager.h"
#include "block.h"
#include "weapon.h"
#include "armour.h"
#include "tool.h"

#include "../utils/confignode.h"
#include "../utils/assert.h"
#include <iostream>

ItemDefManager::ItemDefManager(const std::string& filename){
	int id = 0;

	ConfigNode::load(filename, [&] (ConfigNode& configArray){
		configArray.forEachNode([&] (ConfigNode& config) {
			ItemDef * stat = nullptr;
			std::string className = config.getString("class");

#define OPTION(str,class) if (className == str) {stat = new class ();}
			OPTION("block",Block)
			OPTION("tool",Tool)
			OPTION("weapon",Weapon)
			OPTION("armour",Armour)
#undef OPTION

			ASSERT(stat, filename, "Unknown class " + className);
			if (stat != nullptr){
				stat->ID = id;
				id++;
				stat->load(config);
				itemDefs.push_back(stat);
			}
		});
	});
}
