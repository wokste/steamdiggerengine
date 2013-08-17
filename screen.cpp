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
#include "screen.h"
#include "entities/entity.h"
#include "enums.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <SFML/Window.hpp>
#include <iostream>

constexpr double eyedist = 10;
constexpr double blocksX = 30;
constexpr double blocksY = 18;
constexpr double nearPlane = eyedist - 3;
constexpr double farPlane = eyedist + 3;

int floorInt(double);

Screen::Screen(){
	sf::ContextSettings settings;
	settings.depthBits = 0;
	settings.stencilBits = 0;
	settings.antialiasingLevel = 0;
	settings.majorVersion = 1;
	settings.minorVersion = 1;

	window.reset(new sf::Window(sf::VideoMode(1000, 600), "Steamdigger", sf::Style::Default, settings));
	window->setVerticalSyncEnabled(true);

	center = Vector2d(0,0);
	size = Vector2::uToI(window->getSize());
	glViewport(0, 0, size.x, size.y);
}

void Screen::startScene(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	constexpr double factor = (nearPlane / eyedist) / 2;
	glFrustum(-blocksX*factor, blocksX*factor, blocksY*factor, -blocksY*factor, nearPlane, farPlane);
	gluLookAt(0,0,1,0,0,0,0,1,0);
	glMatrixMode(GL_MODELVIEW);
}

void Screen::resize(Vector2i newSize){
	size = newSize;
	glViewport(0, 0, size.x, size.y);
}

void Screen::centerOn(Entity& player){
	center = player.pos;
	glScaled(1,1,-1);
	glTranslated(-center.x,-center.y,eyedist);
}

bool Screen::containsArea(Vector2i topLeft, Vector2i vecSize) const{
	constexpr double factor = (farPlane / eyedist) / 2;
	return (topLeft.x < center.x + blocksX*factor)
		&& (topLeft.x + vecSize.x > center.x - blocksX*factor)
		&& (topLeft.y < center.y + blocksY*factor)
		&& (topLeft.y + vecSize.y > center.y - blocksY*factor);
}

// Magical function inspired by http://olivers.posterous.com/linear-depth-in-glsl-for-real, but reversed
constexpr double zBufferForLayer(int layer){
	return (
		(farPlane + nearPlane - (2.0 * nearPlane * farPlane) / (eyedist + layer + 1)) // No idea why this +1 is needed
	/
		(farPlane - nearPlane)
	+ 1.0 ) / 2.0;
}

Vector2d Screen::mousePos(const int layer) const{
	auto mouse = sf::Mouse::getPosition(*window);

    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );

    winX = (double)mouse.x;
    winY = (double)size.y - (double)mouse.y;
    if (layer == -1)
		glReadPixels( mouse.x, mouse.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
	else
		winZ = zBufferForLayer(layer);

    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
    return Vector2d(posX + center.x, posY + center.y);
}

int Screen::getSelectedLayer() const{
	return
		(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
		? Layer::back
		: Layer::front;
}
