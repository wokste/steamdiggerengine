#pragma once
#include "utils/vector2.h"

namespace sf{
class Window;
}

class Entity;

class Screen{
public:
	sf::Window* window;
	Screen(sf::Window* newWindow);
	void startScene();
	void resize(Vector2i newPos);
	void centerOn(Entity * player);

	// Note that openGL uses a different coordinate system than we do.
	Vector2d mousePos(int layer = -1);

protected:
	Vector2d center;
	Vector2i size;
};
