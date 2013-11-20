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
#include "src/hud/hud.h"
#include "src/hud/inventoryhud.h"
#include "src/entities/player.h"
#include "src/items/itemdefmanager.h"
#include "src/items/item.h"
#include "src/utils/texture.h"
#include "src/screen.h"
#include "src/game.h"
#include "src/utils/font.h"
#include <SFML/OpenGL.hpp>
#include <sstream>

InventoryHUD::InventoryHUD(){
	background.reset(new Texture(GameGlobals::fileSystem.fullpath("inventoryhud.png")));
	itemTexture = GameGlobals::tileSet;
	isOpen = true;
	toggle();
	docking = Vector2d(0.5, 1);
	slotMarkerSize = Vector2i(46,46);
}

InventoryHUD::~InventoryHUD(){
}

void InventoryHUD::draw(const Player& player){
	background->bind();
	int rowOffset;

	// Draw background

	background->draw(Vector2i(0, backgroundTop), size, Vector2i(0, 0));
	background->draw(Vector2i(406, 0), slotMarkerSize, Vector2i((celSize + celBorder) * player.inventory.selectedItem, size.y - slotMarkerSize.y));

	rowOffset = Inventory::height - rows;
	// Drawing items
	itemTexture->bind();
	for (int x = 0; x < Inventory::width;++x){
		for (int y = 0; y < rows;++y){
			int id = x + (3 - y - rowOffset) * Inventory::width;
			if (player.inventory.items[id].count > 0){
				int framesPerRow = itemTexture->size.x / celSize;
				int frame = (*GameGlobals::itemDefs)[player.inventory.items[id].id].frameID;
				itemTexture->draw(Vector2i((frame % framesPerRow), (frame / framesPerRow)) * 32, Vector2i(32,32), Vector2i(x * 40 + 7, y * 40 + 7));
			}
		}
	}

	// Drawing the text
	HUD::font.texture->bind();
	for (int x = 0; x < Inventory::width;++x){
		for (int y = 0; y < rows;++y){
			int id = x + (3 - y - rowOffset) * Inventory::width;
			if (player.inventory.items[id].count > 1){
				std::stringstream text;
				text << player.inventory.items[id].count;
				HUD::font.draw(text.str(),Rect4i(x * (celSize + celBorder) + outsideBorder, y * (celSize + celBorder) + outsideBorder,celSize,celSize));
			}
		}
	}
}

void InventoryHUD::toggle(){
	isOpen = !isOpen;

	cols = Inventory::width;
	rows = (isOpen) ? (Inventory::height) : 1;
	backgroundTop = (isOpen) ? 0 : (Inventory::height * celSize + (Inventory::height - 1) * celBorder + 2 * outsideBorder);
	size = Vector2i(cols * celSize + (cols - 1) * celBorder + 2 * outsideBorder,
	                rows * celSize + (rows - 1) * celBorder + 2 * outsideBorder);

}

bool InventoryHUD::onMouseEvent(sf::Event& event, Player& player, const Vector2i& mousePos){
	int celX = (mousePos.x - outsideBorder + celBorder / 2) / (celSize + celBorder);
	int celY = (mousePos.y - outsideBorder + celBorder / 2) / (celSize + celBorder);
	celY = rows - celY - 1;
	if (celX < 0 || celX >= cols || celY < 0 || celY > rows)
		return false;
	int cel = celX + (celY * cols);
	if (event.type == sf::Event::EventType::MouseButtonPressed)
		std::swap(player.inventory.items[cel], player.inventory.mouseHeldItem);
	return true;
}
