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

Texture::Texture(std::string fileName, Vector2i newFrameSize){
	if (loadTexture(fileName, newFrameSize)){
		std::cout<<"loaded texture " << fileName << " as " << ID<<'\n';
		return;
	}
	ID = 0;
	setSize(newFrameSize, newFrameSize);
	std::cout<<"error loading texture " << fileName << '\n';
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

	glColor3ub((unsigned char)(color >> 16), (unsigned char)(color >> 8), (unsigned char)(color));
}

void Texture::drawTile(Vector2d pos, Vector2i tile, int tileNum) const{
	double texLeft = (tileNum % framesPerRow) * sizeGL.x;
	double texRight = texLeft + sizeGL.x;
	double texTop = (tileNum / framesPerRow) * sizeGL.y;
	double texBottom = texTop + sizeGL.y;

	Vector2d tileD = Vector2::iToD(tile) / 16.0;

	glBegin(GL_QUADS);
	glTexCoord2d(texLeft, texTop);
	glVertex3d(pos.x, pos.y, 0.5);
	glTexCoord2d(texLeft, texBottom);
	glVertex3d(pos.x, pos.y + tileD.y, 0.5);
	glTexCoord2d(texRight, texBottom);
	glVertex3d(pos.x + tileD.x, pos.y + tileD.y, 0.5);
	glTexCoord2d(texRight, texTop);
	glVertex3d(pos.x + tileD.x, pos.y, 0.5);
	glEnd();
}

void Texture::drawBlock(Vector3i pos, int tileNum) const{
	double x = pos.x;
	double y = pos.y;
	double z = pos.z;

	double texLeft = (tileNum % framesPerRow) * sizeGL.x;
	double texRight = texLeft + sizeGL.x;
	double texTop = (tileNum / framesPerRow) * sizeGL.y;
	double texBottom = texTop + sizeGL.y;

	glBegin(GL_QUADS);
		// Front Face
		glTexCoord2d(texLeft, texTop);    glVertex3d(x    ,y    ,z+1.0);
		glTexCoord2d(texRight, texTop);   glVertex3d(x+1.0,y    ,z+1.0);
		glTexCoord2d(texRight, texBottom);glVertex3d(x+1.0,y+1.0,z+1.0);
		glTexCoord2d(texLeft, texBottom); glVertex3d(x    ,y+1.0,z+1.0);
		// Back Face
		glTexCoord2d(texRight, texTop);   glVertex3d(x    ,y    ,z);
		glTexCoord2d(texRight, texBottom);glVertex3d(x    ,y+1.0,z);
		glTexCoord2d(texLeft, texBottom); glVertex3d(x+1.0,y+1.0,z);
		glTexCoord2d(texLeft, texTop);    glVertex3d(x+1.0,y    ,z);
		// Top Face
		glTexCoord2d(texLeft, texBottom); glVertex3d(x    ,y+1.0,z);
		glTexCoord2d(texLeft, texTop);    glVertex3d(x    ,y+1.0,z+1.0);
		glTexCoord2d(texRight, texTop);   glVertex3d(x+1.0,y+1.0,z+1.0);
		glTexCoord2d(texRight, texBottom);glVertex3d(x+1.0,y+1.0,z);
		// Bottom Face
		glTexCoord2d(texRight, texBottom);glVertex3d(x    ,y    ,z);
		glTexCoord2d(texLeft, texBottom); glVertex3d(x+1.0,y    ,z);
		glTexCoord2d(texLeft, texTop);    glVertex3d(x+1.0,y    ,z+1.0);
		glTexCoord2d(texRight, texTop);   glVertex3d(x    ,y    ,z+1.0);
		// Right face
		glTexCoord2d(texRight, texTop);   glVertex3d(x+1.0,y    ,z);
		glTexCoord2d(texRight, texBottom);glVertex3d(x+1.0,y+1.0,z);
		glTexCoord2d(texLeft, texBottom); glVertex3d(x+1.0,y+1.0,z+1.0);
		glTexCoord2d(texLeft, texTop);    glVertex3d(x+1.0,y    ,z+1.0);
		// Left Face
		glTexCoord2d(texLeft, texTop);    glVertex3d(x    ,y    ,z);
		glTexCoord2d(texRight, texTop);   glVertex3d(x    ,y    ,z+1.0);
		glTexCoord2d(texRight, texBottom);glVertex3d(x    ,y+1.0,z+1.0);
		glTexCoord2d(texLeft, texBottom); glVertex3d(x    ,y+1.0,z);
	glEnd();
}

void Texture::drawPart(Vector2i src, Vector2i imgSize, Vector2i dest) const{
	double texLeft = ((double)src.x / size.x);
	double texRight = ((double)(src.x + imgSize.x) / size.x);
	double texTop = ((double)src.y / size.y);
	double texBottom = ((double)(src.y + imgSize.y) / size.y);

	//imgSize /= 16; // Quickfix. Remove this

	glBegin(GL_QUADS);
	glTexCoord2d(texLeft,  texTop);    glVertex2d(dest.x            , dest.y);
	glTexCoord2d(texLeft,  texBottom); glVertex2d(dest.x            , dest.y + imgSize.y);
	glTexCoord2d(texRight, texBottom); glVertex2d(dest.x + imgSize.x, dest.y + imgSize.y);
	glTexCoord2d(texRight, texTop);    glVertex2d(dest.x + imgSize.x, dest.y);
	glEnd();
}

Texture::~Texture(){
	std::cout<<"unloaded texture " << ID <<'\n';
	glDeleteTextures(1, &ID);
}

bool Texture::loadTexture(std::string fileName, Vector2i newFrameSize){
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

	setSize(Vector2i(imgSize.x , imgSize.y), newFrameSize);
	return true;
}
