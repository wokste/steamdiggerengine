#include "screen.h"
#include "entities/entity.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <SFML/Window.hpp>
#include <iostream>

constexpr double eyedist = 8;
constexpr double blocksOnScreen = 50;
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

	window.reset(new sf::Window(sf::VideoMode(800, 600), "Steamdigger", sf::Style::Default, settings));
	window->setVerticalSyncEnabled(true);

	center = Vector2d(0,0);
	size = Vector2::uToI(window->getSize());
	glViewport(0, 0, size.x, size.y);
}

void Screen::startScene(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	double blocksX = sqrt(blocksOnScreen * size.x / size.y);
	double blocksY = sqrt(blocksOnScreen * size.y / size.x);
	glFrustum(-blocksX/2, blocksX/2, blocksY/2, -blocksY/2, nearPlane, farPlane);
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
	glScaled(1,1,-1);
	glTranslated(-center.x,-center.y,eyedist);

	/*gluLookAt(center.x, center.y, 0.0f,
		center.x, center.y, 1.0f,
		0.0f, 1.0f,  0.0f);*/
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
