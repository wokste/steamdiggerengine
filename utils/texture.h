#pragma once

#include <GL/gl.h>
#include <string>
#include "vector2.h"

class ConfigNode;

struct Texture{
	GLuint ID;
	Vector2i size;

	static Texture* load(const ConfigNode& config);
	Texture(std::string fileName);
	~Texture();
	bool loadTexture(std::string fileName);
	void bind();
	void draw(Vector2i src, Vector2i imgSize, Vector2i dest) const;
	void draw(Vector2i src, Vector2i srcSize, Vector2d dest, Vector2d destSize) const;
};
