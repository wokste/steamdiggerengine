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
protected:
	Vector2d center;
	Vector2i size;
};
