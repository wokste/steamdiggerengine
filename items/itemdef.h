#pragma once
#include "../utils/vector2.h"

class Player;
class ItemStack;
class Block;
class Weapon;
class Armour;
class Tool;

enum class ItemType{
	IT_Item,
	IT_Weapon,
	IT_Armour,
	IT_Tool,
	IT_Block
};

class ItemDef{
protected:
	ItemType itemType;
public:
	virtual bool use(Player& owner, ItemStack& item, Vector2i mousePos) {return false;}
};
