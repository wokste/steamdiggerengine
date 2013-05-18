#pragma once;
#include "../utils/vector2.h"

class Block;
class ItemDefManager;
class Texture;

typedef int Color;

class MapNode{
public:
	MapNode();
	void setBlock(Block* block, int layer);
	Block* getBlock(ItemDefManager* itemDefs, int layer);
	void render(Texture& tileset, Vector2i pos);
	bool isset(int layer){ return blockId[layer] != -1;}
	Color light[2];
private:
	static constexpr int MAX_LAYERS = 2;
	int frame[MAX_LAYERS];
	int blockId[MAX_LAYERS];
};
