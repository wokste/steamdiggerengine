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
#include "hud.h"
#include "../entities/player.h"
#include "../items/itemdefmanager.h"
#include "../items/item.h"
#include "../utils/texture.h"
#include "../screen.h"
#include "../game.h"
#include <GL/gl.h>

/* *******
   * HUD *
   ******* */

#include "font.h"
#include <pugixml.hpp>
#include <sstream>
#include <iostream>

Font HUD::font;

HUD::HUD(){
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
				Vector2d pos = Vector2::iToD(screen.getSize() - elem->size);
				pos.x *= elem->docking.x;
				pos.y *= elem->docking.y;
				glTranslated(pos.x, pos.y, 0);
				elem->draw(player);
			}
			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

void HUD::toggleInventory(){
	for (auto& elem: hudElements){
		auto inventoryElem = dynamic_cast<InventoryHUD*>(elem.get());
		if (inventoryElem){
			inventoryElem->toggle();
		}
	}
}

bool HUD::onMousePressed(const Screen& screen, Player& player, const sf::Mouse::Button& button, const Vector2i mousePos){
	for (int i = hudElements.size() - 1; i >= 0; --i){
		auto& elem = hudElements[i];
		Vector2d pos = Vector2::iToD(screen.getSize() - elem->size);
		pos.x *= elem->docking.x;
		pos.y *= elem->docking.y;
		Vector2i relativeMousePos = mousePos - Vector2::floorVec(pos);
		if (relativeMousePos.x < 0 || relativeMousePos.y < 0 || relativeMousePos.x >= elem->size.x || relativeMousePos.y >= elem->size.y)
			continue;

		if (elem->onMousePressed(player, button, relativeMousePos))
			return true;
	}
	return false;
}

/* ***************
   * Hud Element *
   *************** */

bool HUDElement::onMousePressed(Player& player, const sf::Mouse::Button& button, const Vector2i mousePos){
	return true;
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
