#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

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
	Texture(std::string fileName, Vector2i frameSize);
	~Texture();
	bool loadTexture(std::string fileName, Vector2i frameSize);
	void bind();

	void setSize(Vector2i imgSize, Vector2i frameSize);
	void drawTile(Vector2d pos, Vector2i tileSize, int tileNum) const;
	void drawBlock(Vector3i pos, int tileNum) const;
	void drawPart(Vector2i src, Vector2i imgSize, Vector2i dest) const;
};

#endif // TEXTURE_H
