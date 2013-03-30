#pragma once
#include <string>

class Texture;
class Screen;

class Map{
public:
	class Tile{
	public:
		int8_t frame;
		int16_t num;
	};

	int		  mapWidth, mapHeight;
	int		  tileWidth, tileHeight;
	double	   gravity = 200;

public:
	Map();
	~Map();

	bool generate(int seed);
	void render();
	void setTile(int x, int y, int16_t value);

private:
	Texture* tileSet;
	Tile* tiles;

	int tileNum(int x, int y);
	Tile* tile(int x, int y);
	int32_t seed;

	void unloadResources();
	void findTileFrame(int x, int y);
public:
	bool validPos(int x1Px, int x2Px, int y1Px, int y2Px);
};
