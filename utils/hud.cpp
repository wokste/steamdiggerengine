#include "hud.h"
#include "../entities/player.h"
#include "../utils/texture.h"
#include <GL/gl.h>

HUD::HUD(){
	barX = 10;
	barY = 10;
	barWidth = 256;
	barHeight = 24;
	barTexture = new Texture("healthbar.png",Vector2i(256,24));
}

HUD::~HUD(){
	if (barTexture != nullptr)
		delete barTexture;
}

void HUD::draw(Player* player){
	glLoadIdentity();
	drawBar(player->HP / player->stats->hP, 0.3);
	drawEffects(player);
}

void HUD::drawEffects(Player* player){
	// TODO: Draw a gray overlay when the player is at negative HP.
	// TODO: Draw a red overlay when the player is hurt.
	// TODO: show some effect when the player is at low health
}

void HUD::drawBar(double HPPerc, double ShieldPerc){
	int widthHP	 = std::max<int>((int) barWidth * HPPerc, 0);
	int widthShield = std::max<int>((int) barWidth * ShieldPerc, 0);

	barTexture->bind(0xffffffff);
	barTexture->drawPart(Vector2i(0, 0), Vector2i(widthHP, barHeight), Vector2i(0, 0));
	barTexture->drawPart(Vector2i(widthHP, barHeight), Vector2i(barWidth - widthHP, barHeight), Vector2i(widthHP, 0));
	barTexture->drawPart(Vector2i(0, 2*barHeight), Vector2i(widthShield, barHeight), Vector2i(0, barHeight));
	barTexture->drawPart(Vector2i(widthShield, 3*barHeight), Vector2i(barWidth - widthShield, barHeight), Vector2i(widthShield, barHeight));
}
