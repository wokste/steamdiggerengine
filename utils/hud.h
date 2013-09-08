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

#include "../utils/vector2.h"
#include <vector>
#include <SFML/Window.hpp>
#include <memory>

class Texture;
class TileSet;
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
	void drawMouseItem(const Screen& screen, const Player& player);
	void toggleInventory();
	bool onMouseEvent(sf::Event& event, const Screen& screen, Player& player);
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
	virtual bool onMouseEvent(sf::Event& event, Player& player, const Vector2i mousePos);
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
	bool onMouseEvent(sf::Event& event, Player& player, const Vector2i mousePos) override;
private:
	std::shared_ptr<Texture> background;
	std::shared_ptr<TileSet> itemTexture;
	bool isOpen;

	Vector2i slotMarkerSize;
	int rows;
	int cols;
	int backgroundTop;
};

