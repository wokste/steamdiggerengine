#include "hud.h"
#include "../entities/player.h"
#include "../utils/texture.h"
#include "../screen.h"
#include "../utils/gamesettings.h"
#include <GL/gl.h>

/* *******
   * HUD *
   ******* */

HUD::HUD(GameSettings* gameSettings){
	hudElements.push_back(new HealthBarHUD(gameSettings));
}

HUD::~HUD(){
	for (auto elem : hudElements)
		delete elem;
}

void HUD::draw(Screen& screen, Player* player){
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();
		glOrtho(0.0, screen.getSize().x, screen.getSize().y, 0.0, -1.0, 10.0);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			glDisable(GL_DEPTH_TEST);
			for (auto elem : hudElements){
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

/* ****************
   * HealthBarHUD *
   **************** */

HealthBarHUD::HealthBarHUD(GameSettings* gameSettings){
	barSize = Vector2i(256, 24);
	barTexture = new Texture(gameSettings->findResource("healthbar.png"),barSize);

	size = Vector2i(256, 48);
	docking = Vector2d(1, 0);
}

HealthBarHUD::~HealthBarHUD(){
	if (barTexture != nullptr)
		delete barTexture;
}

void HealthBarHUD::draw(Player* player){
	double HPPerc = player->HP / player->stats->HP;
	double ShieldPerc = 0.2;

	int widthHP	 = std::max<int>((int) barSize.x * HPPerc, 0);
	int widthShield = std::max<int>((int) barSize.x * ShieldPerc, 0);

	barTexture->bind(0xffffffff);
	barTexture->drawPart(Vector2i(0, 0), Vector2i(widthHP, barSize.y), Vector2i(0, 0));
	barTexture->drawPart(Vector2i(widthHP, barSize.y), Vector2i(barSize.x - widthHP, barSize.y), Vector2i(widthHP, 0));
	barTexture->drawPart(Vector2i(0, 2*barSize.y), Vector2i(widthShield, barSize.y), Vector2i(0, barSize.y));
	barTexture->drawPart(Vector2i(widthShield, 3*barSize.y), Vector2i(barSize.x - widthShield, barSize.y), Vector2i(widthShield, barSize.y));
}
