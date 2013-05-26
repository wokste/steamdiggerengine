#include "hud.h"
#include "../entities/player.h"
#include "../utils/texture.h"
#include "../screen.h"
#include "../game.h"
#include <GL/gl.h>

/* *******
   * HUD *
   ******* */

HUD::HUD(const Game& game){
	hudElements.emplace_back(new HealthBarHUD(game));
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
			for (auto& elem : hudElements){
				// TODO: positioning the hud item
				glLoadIdentity();
				Vector2d pos = Vector2::iToD(screen.getSize() - elem->size);
				pos.x *= elem->docking.x;
				pos.y *= elem->docking.y;
				glTranslated(pos.x, pos.y, 0);
				elem->draw(player);
			}
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

HealthBarHUD::HealthBarHUD(const Game& game){
	barSize = Vector2i(256, 24);
	barTexture.reset(new Texture(game.fileSystem.fullpath("healthbar.png"),barSize));

	size = Vector2i(256, 48);
	docking = Vector2d(1, 0);
}

HealthBarHUD::~HealthBarHUD(){
}

void HealthBarHUD::draw(const Player& player){
	double HPPerc = player.HP / player.stats.HP;
	double ShieldPerc = 0.2;

	int widthHP	 = std::max<int>((int) barSize.x * HPPerc, 0);
	int widthShield = std::max<int>((int) barSize.x * ShieldPerc, 0);

	barTexture->bind();
	barTexture->drawPart(Vector2i(0, 0), Vector2i(widthHP, barSize.y), Vector2i(0, 0));
	barTexture->drawPart(Vector2i(widthHP, barSize.y), Vector2i(barSize.x - widthHP, barSize.y), Vector2i(widthHP, 0));
	barTexture->drawPart(Vector2i(0, 2*barSize.y), Vector2i(widthShield, barSize.y), Vector2i(0, barSize.y));
	barTexture->drawPart(Vector2i(widthShield, 3*barSize.y), Vector2i(barSize.x - widthShield, barSize.y), Vector2i(widthShield, barSize.y));
}
