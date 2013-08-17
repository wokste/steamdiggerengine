#pragma once

#include "../utils/vector2.h"
#include <vector>
#include <SFML/Window.hpp>
#include <memory>

class Texture;
class Screen;
class Player;
class HUDElement;
class Font;

class HUD{
public:
	HUD();
	HUD(const HUD& that) = delete;
	~HUD();

	void draw(const Screen& screen, const Player& player);
	void toggleInventory();
	bool onMousePressed(const Screen& screen, Player& player, const sf::Mouse::Button& button, const Vector2i mousePos);
	static Font font;

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

class InventoryHUD : public HUDElement{
public:
	static constexpr int celSize = 32, celBorder = 8, outsideBorder = 7;
	int rowsShown = 1;

	InventoryHUD();
	InventoryHUD(const InventoryHUD& that) = delete;
	virtual ~InventoryHUD();

	virtual void draw(const Player& player);
	void toggle();
private:
	std::shared_ptr<Texture> background;
	std::shared_ptr<Texture> itemTexture;
	bool isOpen;

	Vector2i slotMarkerSize;
	int rows;
	int cols;
	int backgroundTop;
};

