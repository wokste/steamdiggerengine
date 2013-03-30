#pragma once
#include <vector>
class ItemDef;

class ItemDefManager{
	std::vector<ItemDef*> itemDefs;
public:
	ItemDefManager();
	ItemDef* getItemDef(int id);
};
