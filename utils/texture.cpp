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
#include "texture.h"

#include <stdlib.h>
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <pugixml.hpp>

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

TileSet::TileSet(Vector2i newFramesPerSheet, std::string fileName) :
	Texture (fileName),
	framesPerSheet(newFramesPerSheet)
{}

sf::Rect<float> TileSet::getBounds(int tileNum) const{
	sf::Rect<float> rect;
	rect.left = (float)(tileNum % framesPerSheet.x) / framesPerSheet.x;
	rect.width = (1.0 / framesPerSheet.x);
	rect.top = (float)(tileNum / framesPerSheet.x) / framesPerSheet.y;
	rect.height = (1.0 / framesPerSheet.y);
	return rect;
}
