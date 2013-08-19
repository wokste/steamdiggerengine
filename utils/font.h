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
#include <memory>
#include <vector>
#include <string>
#include "vector2.h"
#include <pugixml.hpp>

class ConfigNode;
class Texture;

struct FontChar{
	int x       :8;
	int y       :8;
	int width   :8;
	int xOffset :8;

	FontChar(pugi::xml_node& node);
	~FontChar();
};

class Font{
	std::vector<FontChar> charsData;
	int height;
	int yOffset;
	int firstID;
	int lastID;

public:
	Font();
	~Font();
	std::shared_ptr<Texture> texture;
	void load(pugi::xml_node& node);
	void draw(const std::string& text, Rect4i area) const;
};
