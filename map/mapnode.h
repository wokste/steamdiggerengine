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
#include "../enums.h"
#include "../utils/vector2.h"
#include <SFML/Graphics/Color.hpp>

class Block;
class ItemDefManager;
class Texture;
class Skybox;
class BlockType;
class Map;

class MapNode{
public:
	MapNode();
	void setBlock(int blockId, int layer);
	const BlockType& getBlock(int layer) const;
	void render(const sf::Color& skyColor, Vector2i pos, int focussedLayer) const;
	bool isset(int layer) const{ return blockId[layer] != 0;}
	sf::Color light[2];
	sf::Color getLight(const sf::Color& skyColor) const;
	bool damageBlock(int layer, int damageHigh, int damageLow, int damageTypeSet);
private:
	int modelId[Layer::count];
	int blockId[Layer::count];
	int HP[Layer::count];
};
