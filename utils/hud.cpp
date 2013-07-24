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

HUD::HUD(){
	hudElements.emplace_back(new HealthBarHUD());
	hudElements.emplace_back(new InventoryHUD());
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

void HUD::escapePressed(){

}

bool HUD::onMousePressed(const Screen& screen, Player& player, const sf::Mouse::Button& button, const Vector2i mousePos){
	for (int i = hudElements.size() - 1; i >= 0; --i){
		auto& elem = hudElements[i];
		Vector2d pos = Vector2::iToD(screen.getSize() - elem->size);
		pos.x *= elem->docking.x;
		pos.y *= elem->docking.y;
		Vector2i relativeMousePos = mousePos - Vector2::dToI(pos);
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

#include <iostream>

InventoryHUD::InventoryHUD(){
	background.reset(new Texture(GameGlobals::fileSystem.fullpath("inventoryhud.png")));
	itemTexture.reset(new Texture(GameGlobals::fileSystem.fullpath("tileset.png")));
	isOpen = true;
	toggleOpen();
	docking = Vector2d(0.5, 1);
}

InventoryHUD::~InventoryHUD(){
}

void InventoryHUD::draw(const Player& player){
	background->bind();
	int rows, rowOffset;
	// Draw background
	if (isOpen){
		background->draw(Vector2i(0, 0), size, Vector2i(0, 0));
		rows = Inventory::height;
		rowOffset = 0;
	} else {
		background->draw(Vector2i(0, 165), size, Vector2i(0, 0));
		rows = 1;
		rowOffset = Inventory::height - rows;
	}
	itemTexture->bind();
	for (int x = 0; x < Inventory::width;++x){
		for (int y = 0; y < rows;++y){
			int id = x + (3 - y - rowOffset) * Inventory::width;
			if (player.inventory.items[id].count > 0){
				int framesPerRow = itemTexture->size.x / 32;
				int frame = (*GameGlobals::itemDefs)[player.inventory.items[id].id].frameID;
				itemTexture->draw(Vector2i((frame % framesPerRow), (frame / framesPerRow)) * 32, Vector2i(32,32), Vector2i(x * 40 + 7, y * 40 + 7));
			}
		}
	}
}

void InventoryHUD::toggleOpen(){
	isOpen = !isOpen;
	if (isOpen){
		size = Vector2i(405, 164);
	} else {
		size = Vector2i(405, 46);
	}
}
