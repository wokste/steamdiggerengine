#pragma once
#include <vector>
#include <string>
class ItemDef;
class Texture;
class GameSettings;

class ItemDefManager{
	std::vector<ItemDef*> itemDefs;
public:
	ItemDefManager(GameSettings& gameSettings, const std::string& configFileName);
	ItemDef* operator[](int id){return itemDefs[id];}
	int size() {return itemDefs.size();}
	Texture* textureSet;
};
