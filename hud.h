#pragma once

class Texture;
class Player;

class HUD{
public:
	HUD();
	HUD(const HUD& that) = delete;
	~HUD();

	void draw(Player* player);
	void drawEffects(Player* player);
	void drawBar(double HPPerc, double shieldPerc);

private:
	int barX;
	int barY;
	int barWidth;
	int barHeight;
	Texture* barTexture;
};
