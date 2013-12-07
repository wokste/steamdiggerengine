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
#include "src/hud/statusbarhud.h"
#include "src/hud/inventoryhud.h"
#include "src/items/item.h"
#include "src/items/itemdefmanager.h"
#include "src/utils/filesystem.h"

#include "src/entities/player.h"
#include "src/utils/texture.h"
#include "src/screen.h"
#include <SFML/OpenGL.hpp>

/* *******
   * HUD *
   ******* */

#include "src/utils/font.h"
#include <pugixml.hpp>
#include <sstream>
#include <iostream>

Font HUD::font;


HUD::HUD() : selectedElement(nullptr){
	hudElements.emplace_back(new StatusBarHUD());
	hudElements.emplace_back(new InventoryHUD());

	pugi::xml_document doc;
	auto result = doc.load_file(g_FileSystem.fullpath("font.xml").c_str());

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
	if (player.inventory.mouseHeldItem.count > 0){
		int framesPerRow = g_TileSet->size.x / 32;
		int frame = g_ItemDefs[player.inventory.mouseHeldItem.id].frameID;
		g_TileSet->bind();
		g_TileSet->draw(Vector2i((frame % framesPerRow), (frame / framesPerRow)) * 32, Vector2i(32,32), mousePos);
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
