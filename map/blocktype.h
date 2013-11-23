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
#include "src/enums.h"
#include <vector>
#include <memory>
#include "src/utils/vertex.h"
#include "src/utils/drop.h"
#include <SFML/Graphics/Color.hpp>
#include <pugixml.hpp>
#include "src/utils/attack.h"

class World;
class Sound;


class BlockType{
private:
	int getFlags(std::string text);
public:
	BlockCollisionType collisionType;
	int HP;
	sf::Color lightColor;
	sf::Color blockedLight;
	std::vector<VertexArray> models;

	BlockType(pugi::xml_node& configNode);
	int getModelId() const;

	DropList drops;
	std::shared_ptr<Sound> mineSound;
	int flags;

	enum AttachFlags{
		FLAG_ATTACH_TOP = 1,
		FLAG_ATTACH_BOTTOM = 2,
		FLAG_ATTACH_LEFT = 4,
		FLAG_ATTACH_RIGHT = 8,
		FLAG_ATTACH_FRONT = 16,
		FLAG_ATTACH_BACK = 32,
		FLAG_ATTACH_ADJACENT = FLAG_ATTACH_LEFT | FLAG_ATTACH_RIGHT | FLAG_ATTACH_FRONT | FLAG_ATTACH_BACK
	};
};

class BlockTypeManager{
	std::vector<BlockType> blocks;
public:
	BlockTypeManager(std::string fileName);
	~BlockTypeManager();
	BlockType& operator[](int id);
};
