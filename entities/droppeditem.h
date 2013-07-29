#pragma once
#include "entity.h"

class DroppedItem : public Entity{
public:
	DroppedItem(int itemId, int count = 1);
	virtual ~DroppedItem();
	virtual void logic(double time);
	void render(const sf::Color& skyColor) override;
	virtual void hitTerrain(bool hitWall);
	double TTL; // Time to live in seconds
	int itemId;
	int count;
};
