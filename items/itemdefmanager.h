#pragma once
#include <vector>
#include <string>
#include <memory>

class ItemDef;
class Texture;
class Game;

class ItemDefManager{
	std::vector<std::unique_ptr<ItemDef>> itemDefs;
public:
	ItemDefManager(const Game& game, const std::string& configFileName);
	~ItemDefManager();
	ItemDef* operator[](int id){return itemDefs[id].get();}
	int size() {return itemDefs.size();}
	std::unique_ptr<Texture> textureSet;
};
