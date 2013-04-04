#include "screen.h"
#include "entities/entity.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <SFML/Window.hpp>
#include <iostream>

int floorInt(double);

Screen::Screen(sf::Window* newWindow){
	window = newWindow;
	center = Vector2d(79.8,59.992);
	size = Vector2::uToI(window->getSize());
	glViewport(0, 0, size.x, size.y);
}

void Screen::startScene(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-8,8,6,-6,5,15);      // Left=-2, Right=2, Bottom=-2, Top=2, Near=5, Far=9
	//glFrustum(-size.x / 2,size.x / 2,size.y / 2,-size.y / 2,0.1, 9001);
	//gluPerspective(65.0, 1.5, 0.0, 9001.1337);
	gluLookAt(0,0,1,0,0,0,0,1,0);
	glMatrixMode(GL_MODELVIEW);

}

void Screen::resize(Vector2i newSize){
	size = newSize;
	glViewport(0, 0, size.x, size.y);
}

void Screen::centerOn(Entity * player){
	if (player != nullptr){
		center = player->pos;
	}
	glTranslated(-center.x,-center.y,-10);
	/*gluLookAt(center.x, center.y, 0.0f,
		center.x, center.y, 1.0f,
		0.0f, 1.0f,  0.0f);*/
}

// Note that openGL uses a different coordinate system than we do.
Vector2i Screen::mousePos(){
	// TODO: 3DFIX
	return sf::Mouse::getPosition(*window);// + center - size / 2;
}
