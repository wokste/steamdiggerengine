#ifndef INVENTORY_H
#define INVENTORY_H
#include "../utils/vector2.h"

class ItemDef;
class Player;
class ItemDefManager;

struct ItemStack{
	int id;
	int count;
};

class Inventory{
	ItemStack items[36];
	int selectedItem;
	ItemDefManager* itemDefs;
public:
	Inventory(ItemDefManager* newItemDefs);
	bool use(Player & owner, Vector2i absoluteMouseLoc);
};

#endif // INVENTORY_H
