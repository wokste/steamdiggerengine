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
#include "src/utils/vertex.h"
#include "src/utils/texture.h"
#include <iostream>

Vertex::Vertex(sf::Vector3f pos, float textureX, float textureY) : x(pos.x), y(pos.y), z(pos.z), textureX(textureX), textureY(textureY)
{
};

Vertex operator+(const Vertex& left, const sf::Vector3f& right){
	return Vertex(sf::Vector3f(left.x + right.x, left.y + right.y, left.z + right.z), left.textureX, left.textureY);
};

void VertexArray::clear(){
	vertices.clear();
};

VertexArray::VertexArray(pugi::xml_node& configNode, const TileSet& tileset){
	auto id   = configNode.attribute("id").as_int(-1);
	std::string type = configNode.attribute("display").as_string("cube");
	if (type == "cube"){
		addCube(configNode, tileset);
	}else if (type == "plant"){
		addSheet(sf::Vector3f(0,0,0), sf::Vector3f(0,1,0), sf::Vector3f(1,0,1), id, tileset);
		addSheet(sf::Vector3f(0,0,1), sf::Vector3f(0,1,0), sf::Vector3f(1,0,-1), id, tileset);
	}else if (type == "sprite"){
		addSheet(sf::Vector3f(0,0,0.5), sf::Vector3f(0,1,0), sf::Vector3f(1,0,0), id, tileset);
	} else {
		std::cout << "Warning, undefined display type '" << type << "'\n";
	}
}

void VertexArray::addCube(pugi::xml_node& configNode, const TileSet& tileset){
	const sf::Vector3f base  = sf::Vector3f(0,0,0);
	const sf::Vector3f AxisX = sf::Vector3f(1,0,0);
	const sf::Vector3f AxisY = sf::Vector3f(0,1,0);
	const sf::Vector3f AxisZ = sf::Vector3f(0,0,1);

	auto id   = configNode.attribute("id").as_int(-1);

	addSheet(base        , AxisY, AxisX, configNode.attribute("id-front").as_int(id) , tileset);
	addSheet(base + AxisY, AxisZ, AxisX, configNode.attribute("id-bottom").as_int(id), tileset);
	addSheet(base        , AxisX, AxisZ, configNode.attribute("id-top").as_int(id)   , tileset);
	addSheet(base + AxisX, AxisY, AxisZ, configNode.attribute("id-right").as_int(id) , tileset);
	addSheet(base + AxisZ, AxisY,-AxisZ, configNode.attribute("id-left").as_int(id)  , tileset);
}

void VertexArray::addSheet(const sf::Vector3f start, const sf::Vector3f& dir1, const sf::Vector3f& dir2, int frameNum, const TileSet& tileset){
	auto rect = tileset.getBounds(frameNum);

	vertices.push_back(Vertex(start              , rect.left,              rect.top));
	vertices.push_back(Vertex(start + dir1       , rect.left,              rect.top + rect.height));
	vertices.push_back(Vertex(start + dir2       , rect.left + rect.width, rect.top));

	vertices.push_back(Vertex(start + dir1       , rect.left,              rect.top + rect.height));
	vertices.push_back(Vertex(start + dir1 + dir2, rect.left + rect.width, rect.top + rect.height));
	vertices.push_back(Vertex(start + dir2       , rect.left + rect.width, rect.top));
}

void VertexArray::render() const{
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &(vertices[0].x));
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &(vertices[0].textureX));
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}
