#pragma once

#include "../utils/vector2.h"
#include <vector>
#include <SFML/Window.hpp>
#include <memory>

class Texture;
class Screen;
class Player;
class HUDElement;

class HUD{
public:
	HUD();
	HUD(const HUD& that) = delete;
	~HUD();

	void draw(const Screen& screen, const Player& player);
	void escapePressed();
	bool onMousePressed(const Screen& screen, Player& player, const sf::Mouse::Button& button, const Vector2i mousePos);

private:
	std::vector<std::unique_ptr<HUDElement>> hudElements;
};

class HUDElement{
public:
	HUDElement(){}
	HUDElement(const HUDElement& that) = delete;
	virtual ~HUDElement(){}

	virtual void draw(const Player& player) = 0;
	virtual bool onMousePressed(Player& player, const sf::Mouse::Button& button, const Vector2i mousePos);
	Vector2i size;
	Vector2d docking;
};

class HealthBarHUD : public HUDElement{
public:
	HealthBarHUD();
	HealthBarHUD(const HealthBarHUD& that) = delete;
	virtual ~HealthBarHUD();

	virtual void draw(const Player& player);
private:
	Vector2i barSize;
	std::shared_ptr<Texture> barTexture;
};
