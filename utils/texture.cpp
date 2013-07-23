#include "texture.h"

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "../utils/confignode.h"

extern std::string dataDirectory;

Texture::Texture(std::string fileName){
	if (loadTexture(fileName)){
		std::cout<<"loaded texture " << fileName << " as " << ID<<'\n';
		return;
	}
	ID = 0;
	std::cout<<"error loading texture " << fileName << '\n';
}

void Texture::bind(){
	glBindTexture(GL_TEXTURE_2D, ID);
	gluErrorString(glGetError());
}

void Texture::draw(Vector2i src, Vector2i imgSize, Vector2i dest) const{
	double texLeft = ((double)src.x / size.x);
	double texRight = ((double)(src.x + imgSize.x) / size.x);
	double texTop = ((double)src.y / size.y);
	double texBottom = ((double)(src.y + imgSize.y) / size.y);

	glBegin(GL_QUADS);
	glTexCoord2d(texLeft,  texTop);    glVertex2d(dest.x            , dest.y);
	glTexCoord2d(texLeft,  texBottom); glVertex2d(dest.x            , dest.y + imgSize.y);
	glTexCoord2d(texRight, texBottom); glVertex2d(dest.x + imgSize.x, dest.y + imgSize.y);
	glTexCoord2d(texRight, texTop);    glVertex2d(dest.x + imgSize.x, dest.y);
	glEnd();
}

void Texture::draw(Vector2i src, Vector2i srcSize, Vector2d dest, Vector2d destSize) const{
	double texLeft = ((double)src.x / size.x);
	double texRight = ((double)(src.x + srcSize.x) / size.x);
	double texTop = ((double)src.y / size.y);
	double texBottom = ((double)(src.y + srcSize.y) / size.y);

	glBegin(GL_QUADS);
	glTexCoord2d(texLeft,  texTop);    glVertex2d(dest.x             , dest.y);
	glTexCoord2d(texLeft,  texBottom); glVertex2d(dest.x             , dest.y + destSize.y);
	glTexCoord2d(texRight, texBottom); glVertex2d(dest.x + destSize.x, dest.y + destSize.y);
	glTexCoord2d(texRight, texTop);    glVertex2d(dest.x + destSize.x, dest.y);
	glEnd();
}

Texture::~Texture(){
	glDeleteTextures(1, &ID);
}

bool Texture::loadTexture(std::string fileName){
	sf::Image image;
	if (!image.loadFromFile(fileName))
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

	size = Vector2i(imgSize.x , imgSize.y);
	return true;
}
