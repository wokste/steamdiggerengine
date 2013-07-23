#pragma once

#include <GL/gl.h>
#include <string>
#include "vector2.h"

class ConfigNode;

class Texture{
private:
	GLuint ID;
	Vector2i size;
	Vector2i frameSize;
	sf::Vector2f sizeGL;
	int framesPerRow;
	//bool bAlpha;

public:
	static Texture* load(const ConfigNode& config);
	Texture(std::string fileName);
	~Texture();
	bool loadTexture(std::string fileName);
	void bind();
	void draw(Vector2i src, Vector2i imgSize, Vector2i dest) const;
	void draw(Vector2i src, Vector2i srcSize, Vector2d dest, Vector2d destSize) const;
};
