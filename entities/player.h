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
	Player* spawn(World* world, Vector2d pos);
	virtual void load(const Game& game, const ConfigNode& config);
};

class Player : public Entity{
public:
	Player(World* world, Vector2d newPos, PlayerStats& stats);
	virtual ~Player();
	virtual void logic(double time);
	virtual void checkInput(double time, Screen& screen);
	virtual void takeDamage(Attack& attack, Vector2d source);

	void onMouseWheel(int delta);
	void selectItem(int nr);
	bool useItem(Screen& screen);
	void tryJump();
	Inventory inventory;
private:
	int RP = 0;

	void checkKeyboardMovement(double time);
};
