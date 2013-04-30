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
	void centerOn(Entity * player);

	// Note that openGL uses a different coordinate system than we do.
	Vector2d mousePos(int layer = -1);
	Vector2i getSize(){return size;}
protected:
	Vector2d center;
	Vector2i size;
};
