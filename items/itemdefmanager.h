#pragma once
#include <vector>
#include <string>
class ItemDef;
class Texture;
class Game;

class ItemDefManager{
	std::vector<ItemDef*> itemDefs;
public:
	ItemDefManager(const Game& game, const std::string& configFileName);
	ItemDef* operator[](int id){return itemDefs[id];}
	int size() {return itemDefs.size();}
	Texture* textureSet;
};
