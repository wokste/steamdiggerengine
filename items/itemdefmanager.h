#pragma once
#include <vector>
#include <map>
#include <string>
#include <memory>

class ItemType;
class Texture;
class Game;
class BlockType;

class ItemDefManager{
	std::vector<std::unique_ptr<ItemType>> items;
	std::map<std::string, int> tags;
public:
	ItemDefManager(const std::string& configFileName);
	~ItemDefManager();
	ItemType& operator[](int id){return *items[id];}
	int at(std::string& tag){return tags.at(tag);}
	int insert(std::unique_ptr<ItemType> type, const std::string tag);
	int size() {return items.size();}
	std::unique_ptr<Texture> textureSet;

	int addBuildingBlock(int blockID, int frameID, const std::string tag = "");
};
