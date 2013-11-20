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
#include "src/hud/hudelement.h"
#include "src/entities/player.h"
#include "src/items/itemdefmanager.h"
#include "src/items/item.h"
#include "src/utils/texture.h"
#include "src/screen.h"
#include "src/game.h"
#include <SFML/OpenGL.hpp>

#include "src/utils/font.h"
#include <pugixml.hpp>
#include <sstream>
#include <iostream>

bool HUDElement::onMouseEvent(sf::Event& event, Player& player, const Vector2i& mousePos){
	return false;
}

Vector2i HUDElement::getTopLeft(const Vector2i& screenSize){
	Vector2i pos = screenSize - size;
	pos.x *= docking.x;
	pos.y *= docking.y;
	return pos;
}

bool HUDElement::mouseInArea(const Screen& screen, const Vector2i& mousePos){
	Vector2i relMouse = mousePos - getTopLeft(screen.getSize());
	return (relMouse.x >= 0 && relMouse.y >= 0 && relMouse.x < size.x && relMouse.y < size.y);
}
