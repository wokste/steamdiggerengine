/*
The MIT License (MIT)

Copyright (c) 2013, Steven Wokke

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
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
	double    gravity = 25;

public:
	Map(int seed);
	Map(const Map&) = delete;
	~Map();

	void generateAround(const Vector2d& pos);
	void render(const Screen& screen, const sf::Color& outsideColor) const;
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
