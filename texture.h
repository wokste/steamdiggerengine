#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/gl.h>
#include <string>
#include "utils/vector2.h"

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
	static Texture* load(ConfigNode& config);
	Texture(std::string fileName, Vector2i frameSize);
	~Texture();
	bool loadTexture(std::string fileName, Vector2i frameSize);
	void bind(int color);

	void setSize(Vector2i imgSize, Vector2i frameSize);
	void drawTile(Vector2i pos, Vector2i tileSize, int tileNum);
	void drawPart(Vector2i src, Vector2i imgSize, Vector2i dest);
};

#endif // TEXTURE_H