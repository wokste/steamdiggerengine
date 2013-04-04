#include "texture.h"

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "utils/confignode.h"

extern std::string dataDirectory;

Texture* Texture::load(ConfigNode& config){
	auto fileName = config.getString("texture");
	std::cout << fileName;
	Vector2i frameSize = config.getVector2i("size");
	return new Texture(fileName, frameSize);
}

Texture::Texture(std::string fileName, Vector2i newFrameSize){
	if (!loadTexture(fileName, newFrameSize)){
		ID = 0;
		setSize(newFrameSize, newFrameSize);
		std::cout<<"error loading texture " << fileName << '\n';
		return;
	}
	std::cout<<"loaded texture " << fileName << " as " << ID<<'\n';
}

void Texture::setSize(Vector2i imgSize, Vector2i newFrameSize){
	size = imgSize;
	frameSize = newFrameSize;
	sizeGL = sf::Vector2f((float)(frameSize.x) / size.x,(float)(frameSize.y) / size.y);
	framesPerRow = (size.x / frameSize.x);
}

void Texture::bind(int color){
	glBindTexture(GL_TEXTURE_2D, ID);
	gluErrorString(glGetError());

	glColor4ub((unsigned char)(color >> 24), (unsigned char)(color >> 16), (unsigned char)(color >> 8), (unsigned char)(color));
}

void Texture::drawTile(Vector2d pos, Vector2i tile, int tileNum){
	double texLeft = (tileNum % framesPerRow) * sizeGL.x;
	double texRight = texLeft + sizeGL.x;
	double texTop = (tileNum / framesPerRow) * sizeGL.y;
	double texBottom = texTop + sizeGL.y;

	Vector2d tileD = Vector2::iToD(tile) / 16.0;

	glBegin(GL_QUADS);
	glTexCoord2d(texLeft, texTop);
	glVertex2d(pos.x, pos.y);
	glTexCoord2d(texLeft, texBottom);
	glVertex2d(pos.x, pos.y + tileD.y);
	glTexCoord2d(texRight, texBottom);
	glVertex2d(pos.x + tileD.x, pos.y + tileD.y);
	glTexCoord2d(texRight, texTop);
	glVertex2d(pos.x + tileD.x, pos.y);
	glEnd();
}
void Texture::drawPart(Vector2i src, Vector2i imgSize, Vector2i dest){
	double texLeft = ((double)src.x / size.x);
	double texRight = ((double)(src.x + imgSize.x) / size.x);
	double texTop = ((double)src.y / size.y);
	double texBottom = ((double)(src.y + imgSize.y) / size.y);

	imgSize /= 16; // Quickfix. Remove this

	glBegin(GL_QUADS);
	glTexCoord2d(texLeft, texTop);
	glVertex3d(dest.x, dest.y, 10);
	glTexCoord2d(texLeft, texBottom);
	glVertex3d(dest.x, dest.y + imgSize.y, 10);
	glTexCoord2d(texRight, texBottom);
	glVertex3d(dest.x + imgSize.x, dest.y + imgSize.y, 10);
	glTexCoord2d(texRight, texTop);
	glVertex3d(dest.x + imgSize.x, dest.y, 10);
	glEnd();
}

Texture::~Texture(){
	std::cout<<"unloaded texture " << ID <<'\n';
	glDeleteTextures(1, &ID);
}

bool Texture::loadTexture(std::string fileName, Vector2i frameSize){
	sf::Image image;
	if (!image.loadFromFile(dataDirectory + fileName))
		return false;

	sf::Vector2u imgSize = image.getSize();

	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, imgSize.x, imgSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	setSize(Vector2i(imgSize.x , imgSize.y), frameSize);
	return true;
}
