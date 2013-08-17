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
#include "mapnode.h"

class MapGenerator;

class Chunk{
public:
	static constexpr int width = 16; // a power of 2
	static constexpr int widthMask = width - 1;

	static constexpr int height = 16; // a power of 2
	static constexpr int heightMask = height - 1;
	MapNode nodes[width][height];

	Chunk(const MapGenerator& generator, const Vector2i& topLeft);
	void render(const sf::Color& skyColor, const Vector2i& topLeft, int focussedLayer) const;
};

// == Constraints ==
static_assert(!(Chunk::width  & Chunk::widthMask),  "Chunk::width must be a power of 2");
static_assert(!(Chunk::height & Chunk::heightMask), "Chunk::height must be a power of 2");
