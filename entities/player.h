#pragma once
#include "entity.h"
#include "../items/inventory.h"
#include "../cooldown.h"

class Screen;

class Player : public Entity{
public:
	Player();
	Player(Player& prototype, World* newWorld, Vector2d newPos);
	virtual ~Player();
	virtual void logic(double time);
	virtual void checkInput(double time, Screen& screen);
	virtual void takeDamage(Attack& attack, Vector2d source);

	void onMouseWheel(int delta);
	void selectItem(int nr);
	bool useItem(Screen& screen);
	void tryJump();

	virtual void load(const ConfigNode& config);

	Inventory inventory;
	double jumpHeight;
	double accelSpeed;
	double walkSpeed;
private:
	int RP = 0;

	void checkKeyboardMovement(double time);
};
