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
#include "src/utils/hud.h"
#include "src/entities/player.h"
#include "src/items/itemdefmanager.h"
#include "src/items/item.h"
#include "src/utils/texture.h"
#include "src/screen.h"
#include "src/game.h"
#include <SFML/OpenGL.hpp>

/* *******
   * HUD *
   ******* */

#include "font.h"
#include <pugixml.hpp>
#include <sstream>
#include <iostream>

Font HUD::font;


HUD::HUD() : selectedElement(nullptr){
	hudElements.emplace_back(new HealthBarHUD());
	hudElements.emplace_back(new InventoryHUD());

	pugi::xml_document doc;
	auto result = doc.load_file(GameGlobals::fileSystem.fullpath("font.xml").c_str());

	if (result){
		auto root = doc.child("font");
		font.load(root);
	} else {
		std::cout << "Error description: " << result.description() << "\n";
		std::cout << "Error offset: " << result.offset << "\n\n";
	}
}

HUD::~HUD(){
}

void HUD::draw(const Screen& screen, const Player& player){
	glColor3ub(255,255,255);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();
		glOrtho(0.0, screen.getSize().x, screen.getSize().y, 0.0, -1.0, 10.0);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			for (auto& elem : hudElements){
				glLoadIdentity();
				Vector2i pos = elem->getTopLeft(screen.getSize());
				glTranslatef(pos.x, pos.y, 0);
				elem->draw(player);
			}
			glLoadIdentity();
			drawMouseItem(screen, player);
			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

void HUD::drawMouseItem(const Screen& screen, const Player& player){
	Vector2i mousePos = sf::Mouse::getPosition(*screen.window);
	GameGlobals::tileSet->bind();
	if (player.inventory.mouseHeldItem.count > 0){
		int framesPerRow = GameGlobals::tileSet->size.x / 32;
		int frame = (*GameGlobals::itemDefs)[player.inventory.mouseHeldItem.id].frameID;
		GameGlobals::tileSet->draw(Vector2i((frame % framesPerRow), (frame / framesPerRow)) * 32, Vector2i(32,32), mousePos);
	}
}

bool HUD::hasFocus(){
	return selectedElement != nullptr;
}

void HUD::toggleInventory(){
	for (auto& elem: hudElements){
		auto inventoryElem = dynamic_cast<InventoryHUD*>(elem.get());
		if (inventoryElem){
			inventoryElem->toggle();
		}
	}
}

void HUD::selectElement(const Screen& screen, const Vector2i& mousePos){
	selectedElement = nullptr;
	for (auto& elem: hudElements){
		if (elem->mouseInArea(screen, mousePos)){
			selectedElement = elem.get();
		}
	}
}

bool HUD::onMouseEvent(sf::Event& event, const Screen& screen, Player& player){
	auto mousePos = Vector2i(event.mouseButton.x, event.mouseButton.y);
	if (event.type == sf::Event::EventType::MouseButtonPressed)
		selectElement(screen, mousePos);

	if (selectedElement != nullptr){
		return (selectedElement->onMouseEvent(event, player, mousePos - selectedElement->getTopLeft(screen.getSize())));
	}
	return false;
}

/* ***************
   * Hud Element *
   *************** */

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

/* ****************
   * HealthBarHUD *
   **************** */

HealthBarHUD::HealthBarHUD(){
	barSize = Vector2i(256,24);
	barTexture.reset(new Texture(GameGlobals::fileSystem.fullpath("healthbar.png")));

	size = Vector2i(256, 48);
	docking = Vector2d(1, 0);
}

HealthBarHUD::~HealthBarHUD(){
}

void HealthBarHUD::draw(const Player& player){
	auto HPPerc = player.HP.asProportion();
	auto ShieldPerc = player.shield.asProportion();

	int widthHP	 = std::max<int>((int) barSize.x * HPPerc, 0);
	int widthShield = std::max<int>((int) barSize.x * ShieldPerc, 0);

	barTexture->bind();
	barTexture->draw(Vector2i(0, 0), Vector2i(widthHP, barSize.y), Vector2i(0, 0));
	barTexture->draw(Vector2i(widthHP, barSize.y), Vector2i(barSize.x - widthHP, barSize.y), Vector2i(widthHP, 0));
	barTexture->draw(Vector2i(0, 2*barSize.y), Vector2i(widthShield, barSize.y), Vector2i(0, barSize.y));
	barTexture->draw(Vector2i(widthShield, 3*barSize.y), Vector2i(barSize.x - widthShield, barSize.y), Vector2i(widthShield, barSize.y));
}

/* *****************
   * Inventory HUD *
   ***************** */

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
