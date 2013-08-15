#pragma once
#include "creature.h"
#include "../items/inventory.h"
#include "../cooldown.h"

class Screen;

class Player : public Creature{
public:
	Player();
	virtual ~Player();
	void logic(double time) override;
	virtual void checkInput(double time, Screen& screen);
	void render(const sf::Color& skyColor) override;
	void takeDamage(const Attack& attack, Vector2d source) override;
	void onMouseWheel(int delta);
	void selectItem(int nr);
	bool useItem(Screen& screen);
	void tryJump();

	virtual void load(const ConfigNode& config);
	bool pickupItem(int id, int count) override;

	Inventory inventory;
	double jumpHeight;
	double accelSpeed;
	double walkSpeed;
private:
	void checkKeyboardMovement(double time);
};
