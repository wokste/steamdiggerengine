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
#pragma once
#include "utils/vector2.h"
#include <memory>

namespace sf{
class Window;
}

class Entity;

class Screen{
public:
	std::unique_ptr<sf::Window> window;
	Screen();
	void startScene();
	void resize(Vector2i newPos);
	void centerOn(Entity& player);

	// Note that openGL uses a different coordinate system than we do.
	// TODO: Function name unclear
	Vector2d mousePos(const int layer = -1) const;
	Vector2i getSize() const {return size;}
	bool containsArea(Vector2i topLeft, Vector2i vecSize) const;
	int getSelectedLayer() const;
protected:
	Vector2d center;
	Vector2i size;
};
