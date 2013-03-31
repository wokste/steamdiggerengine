#pragma once
#include <string>

class Texture;
class Screen;
class ItemDefManager;

class Map{
public:
	class Tile{
	public:
		int frame;
		int blockId;
	};

	int		  mapWidth, mapHeight;
	int		  tileWidth, tileHeight;
	double	   gravity = 400;

public:
	Map(ItemDefManager* newItemDefs);
	~Map();

	ItemDefManager* itemDefs;
	bool generate(int seed);
	void render();
	void setTile(int x, int y, int blockId);
	bool validPos(int x1Px, int x2Px, int y1Px, int y2Px);
	Tile* tile(int x, int y);
private:
	Texture* tileSet;
	Tile* tiles;

	int tileNum(int x, int y);
	int32_t seed;

	void unloadResources();
	void findTileFrame(int x, int y);
};
