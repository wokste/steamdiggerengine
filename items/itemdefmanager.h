#pragma once
#include <vector>
#include <string>
#include <memory>

class ItemType;
class Texture;
class Game;

class ItemDefManager{
	std::vector<std::unique_ptr<ItemType>> items;
public:
	ItemDefManager(const Game& game, const std::string& configFileName);
	~ItemDefManager();
	ItemType& operator[](int id){return *items[id];}
	int size() {return items.size();}
	std::unique_ptr<Texture> textureSet;
};
