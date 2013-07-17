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
