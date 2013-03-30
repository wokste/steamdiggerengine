#include "screen.h"
#include "entities/entity.h"
#include <GL/gl.h>
#include <SFML/Window.hpp>
#include <iostream>

int floorInt(double);

Screen::Screen(sf::Window* newWindow){
	window = newWindow;
	center = Vector2i(-200,-200);
	size = Vector2::uToI(window->getSize());
	glViewport(0, 0, size.x, size.y);
}

void Screen::startScene(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, size.x, size.y, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
}

void Screen::resize(Vector2i newSize){
	size = newSize;
	glViewport(0, 0, size.x, size.y);
}

void Screen::centerOn(Entity * player){
	if (player != nullptr){
		center = Vector2::dToI(player->pos);
	}
	glTranslated(size.x / 2, size.y / 2, 0.0);
	glTranslated(-center.x, -center.y, 0.0);
}

// Note that openGL uses a different coordinate system than we do.
Vector2i Screen::mousePos(){
	return sf::Mouse::getPosition(*window) + center - size / 2;
}
