#pragma once
#include <vector>
#include <string>
class ItemDef;

class ItemDefManager{
	std::vector<ItemDef*> itemDefs;
public:
	ItemDefManager(const std::string& filename);
	ItemDef* operator[](int id){return itemDefs[id];}
	int size() {return itemDefs.size();}
};
