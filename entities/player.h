#pragma once
#include "entity.h"
#include "../items/inventory.h"
#include "../cooldown.h"

class Screen;

struct PlayerStats : public EntityStats{
	double jumpHeight;
	int accelSpeed;
	double walkSpeed;
	PlayerStats() = default;
	virtual Entity* spawn(Vector2d pos);
	virtual void load(ConfigNode& config);
};

class Player : public Entity{
public:
	Player(Vector2d newPos, PlayerStats * stats);
	virtual ~Player();
	virtual void logic(int timeMs);
	virtual void checkInput(int timeMs, Screen& screen);
	virtual void takeDamage(Attack& attack, Vector2d source);

	void onMouseWheel(int delta);

private:
	int RP = 0;
	Inventory inventory;

	void checkKeyboardMovement(int timeMs);
};
