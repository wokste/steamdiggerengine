#pragma once
#include "creature.h"
#include "../items/inventory.h"
#include "../cooldown.h"

class Screen;

class Player : public Creature{
public:
	Player();
	virtual ~Player();
	virtual void logic(double time);
	virtual void checkInput(double time, Screen& screen);

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
	void checkKeyboardMovement(double time);
};
