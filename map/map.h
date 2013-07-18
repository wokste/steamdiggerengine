#pragma once
#include <string>
#include "../utils/vector2.h"
#include "../enums.h"
#include <functional>
#include <memory>
#include <map>
#include <vector>
#include "../utils/vertex.h"

namespace sf{
	class Color;
}

class Texture;
class Screen;
class Block;
class MapGenerator;
class MapNode;
class Chunk;
class BlockType;

struct ChunkSorter{
	bool operator()(const Vector2i& a, const Vector2i& b) const;
};

class Map{
public:
	Vector2i  tileSize;
	double    gravity = 25;

public:
	Map(int seed);
	Map(const Map&) = delete;
	~Map();

	void loadBlocks(std::string fileName);
	void generate();
	void render(const sf::Color& outsideColor) const;
	void logic(double time);
	bool areaHasBlocks(Vector2i px1, Vector2i px2, std::function<bool(const BlockType&)>);
	bool blockAdjacent(int x, int y, int layer, std::function<bool(const BlockType&)>);
	MapNode* getMapNode(int x, int y) const;

	sf::Color getColor(const sf::Color& outsideColor, Vector2d pos) const;
private:
	std::shared_ptr<Texture> tileSet;
	std::map<Vector2i, Chunk*, ChunkSorter> chunks;
	std::unique_ptr<MapGenerator> generator;

	int32_t seed;

	void unloadResources();
};
