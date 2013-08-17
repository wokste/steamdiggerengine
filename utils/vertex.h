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
#include <vector>
#include "vector2.h"
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics/Color.hpp>

struct Vertex
{
	GLfloat x,y,z;
	GLfloat textureX,textureY;

	Vertex(sf::Vector3f, float textureX, float textureY);
};

Vertex operator+(const Vertex& left, const sf::Vector3f& right);

/// Used in chunk and in BlockType.
class VertexArray{
public:
	std::vector<Vertex> vertices;
	VertexArray(){}
	VertexArray(std::string type, int frameNum, Vector2i& framesPerSheet);
	~VertexArray(){}

	void clear();

	void addSheet(const sf::Vector3f start, const sf::Vector3f& dir1, const sf::Vector3f& dir2, int frameNum, Vector2i& framesPerSheet);
	void addCube(sf::Vector3f pos, int frameNum, Vector2i& framesPerSheet);

	void render() const;
};
