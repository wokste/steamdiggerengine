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
#include "vertex.h"

Vertex::Vertex(sf::Vector3f pos, float textureX, float textureY) : x(pos.x), y(pos.y), z(pos.z), textureX(textureX), textureY(textureY)
{
};

Vertex operator+(const Vertex& left, const sf::Vector3f& right){
	return Vertex(sf::Vector3f(left.x + right.x, left.y + right.y, left.z + right.z), left.textureX, left.textureY);
};

void VertexArray::clear(){
	vertices.clear();
};

VertexArray::VertexArray(std::string type, int frameNum, Vector2i& framesPerSheet){
	if (type == "cube"){
		addCube(sf::Vector3f(0,0,0), frameNum, framesPerSheet);
	}else if (type == "plant"){
		addSheet(sf::Vector3f(0,0,0), sf::Vector3f(0,1,0), sf::Vector3f(1,0,1), frameNum, framesPerSheet);
		addSheet(sf::Vector3f(0,0,1), sf::Vector3f(0,1,0), sf::Vector3f(1,0,-1), frameNum, framesPerSheet);
	}
}

void VertexArray::addCube(sf::Vector3f pos, int frameNum, Vector2i& framesPerSheet){
	const sf::Vector3f AxisX = sf::Vector3f(1,0,0);
	const sf::Vector3f AxisY = sf::Vector3f(0,1,0);
	const sf::Vector3f AxisZ = sf::Vector3f(0,0,1);

	// Back Face
	//addSheet(pos + AxisZ, AxisX, AxisY, frameNum, framesPerSheet);

	// Front Face
	addSheet(pos        , AxisY, AxisX, frameNum, framesPerSheet);

	// Bottom Face
	addSheet(pos + AxisY, AxisZ, AxisX, frameNum, framesPerSheet);

	// Top Face
	addSheet(pos        , AxisX, AxisZ, frameNum, framesPerSheet);

	// Right face
	addSheet(pos + AxisX, AxisY, AxisZ, frameNum, framesPerSheet);

	// Left Face
	addSheet(pos + AxisY,-AxisY, AxisZ, frameNum, framesPerSheet);
}

void VertexArray::addSheet(const sf::Vector3f start, const sf::Vector3f& dir1, const sf::Vector3f& dir2, int frameNum, Vector2i& framesPerSheet){
	GLfloat texLeft = (GLfloat)(frameNum % framesPerSheet.x) / framesPerSheet.x;
	GLfloat texRight = texLeft + (1.0 / framesPerSheet.x);
	GLfloat texTop = (GLfloat)(frameNum / framesPerSheet.x) / framesPerSheet.y;
	GLfloat texBottom = texTop + (1.0 / framesPerSheet.y);

	vertices.push_back(Vertex(start              , texLeft , texTop));
	vertices.push_back(Vertex(start + dir1       , texLeft , texBottom));
	vertices.push_back(Vertex(start + dir2       , texRight, texTop));

	vertices.push_back(Vertex(start + dir1       , texLeft , texBottom));
	vertices.push_back(Vertex(start + dir1 + dir2, texRight, texBottom));
	vertices.push_back(Vertex(start + dir2       , texRight, texTop));
}

void VertexArray::render() const{
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &(vertices[0].x));
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &(vertices[0].textureX));
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}
