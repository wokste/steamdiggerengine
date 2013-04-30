#pragma once

#include "../utils/vector2.h"
#include <vector>
#include <SFML/Window.hpp>

class Texture;
class Screen;
class Player;
class HUDElement;
class GameSettings;

class HUD{
public:
	HUD(GameSettings* gameSettings);
	HUD(const HUD& that) = delete;
	~HUD();

	void draw(Screen& screen, Player& player);
	void escapePressed();
	bool onMousePressed(Screen& screen, Player& player, sf::Mouse::Button& button, Vector2i mousePos);

private:
	std::vector<HUDElement*> hudElements;
};

class HUDElement{
public:
	HUDElement(){}
	HUDElement(const HUDElement& that) = delete;
	virtual ~HUDElement(){}

	virtual void draw(Player& player) = 0;
	virtual bool onMousePressed(Player& player, sf::Mouse::Button& button, Vector2i mousePos);
	Vector2i size;
	Vector2d docking;
};

class HealthBarHUD : public HUDElement{
public:
	HealthBarHUD(GameSettings* gameSettings);
	HealthBarHUD(const HealthBarHUD& that) = delete;
	virtual ~HealthBarHUD();

	virtual void draw(Player& player);
private:
	Vector2i barSize;
	Texture* barTexture;
};
