#pragma once
#include "entity.h"
#include "../items/inventory.h"
#include "../cooldown.h"

class Screen;

struct PlayerStats : public EntityStats{
	double jumpHeight;
	double accelSpeed;
	double walkSpeed;
	PlayerStats() = default;
	virtual Entity* spawn(World& world, Vector2d pos);
	virtual void load(GameSettings& gameSettings, ConfigNode& config);
};

class Player : public Entity{
public:
	Player(World& world, Vector2d newPos, PlayerStats * stats);
	virtual ~Player();
	virtual void logic(int timeMs);
	virtual void checkInput(int timeMs, Screen& screen);
	virtual void takeDamage(Attack& attack, Vector2d source);

	void onMouseWheel(int delta);
	void selectItem(int nr);
	void tryJump();
	Inventory inventory;
private:
	int RP = 0;

	void checkKeyboardMovement(int timeMs);
};
