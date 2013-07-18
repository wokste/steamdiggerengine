#include "player.h"
#include <SFML/Window.hpp>
#include "../screen.h"
#include "../attack.h"
#include "../world.h"
#include "../game.h"

#include "../utils/confignode.h"
#include "../items/itemdefmanager.h"
#include <iostream>

Player::Player(){

}

Player::Player(Player& prototype, World* newWorld, Vector2d newPos) : Entity(prototype, newWorld, newPos)
	, inventory(), RP(0)
{
	jumpHeight = prototype.jumpHeight;
	accelSpeed = prototype.accelSpeed;
	walkSpeed  = prototype.walkSpeed;
	RP         = prototype.RP;
}

Player::~Player(){

}

void Player::logic(double time){
	Entity::logic(time);
}

void Player::checkKeyboardMovement(double time){
	bool keyLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
	bool keyRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);

	if (keyLeft && !keyRight){
		speed.x -= (accelSpeed * time);
		if (speed.x < -walkSpeed) speed.x = -walkSpeed;
	} else if (keyRight && !keyLeft){
		speed.x += (accelSpeed * time);
		if (speed.x > walkSpeed) speed.x = walkSpeed;
	} else {
		speed.x = 0;
	}
}

void Player::checkInput(double time, Screen& screen){
	if (HP > 0){
		checkKeyboardMovement(time);
	}else{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
			pos.x = 2;
			pos.y = -2;
			HP = 100;
		}
	}

	if (HP > 0){
		inventory.logic(time);
	}
}

void Player::onMouseWheel(int delta){
	if (delta > 0){
		inventory.selectItem(1, true);
	} else if (delta < 0){
		inventory.selectItem(-1, true);
	}
}

void Player::selectItem(int nr){
	inventory.selectItem(nr, false);
}

bool Player::useItem(Screen& screen){
	return inventory.use(*this, screen);
}

void Player::tryJump(){
	if (!validPos(*world, Vector2d(pos.x, pos.y + 0.1))){
		speed.y = -jumpHeight;
	}
}

void Player::takeDamage(Attack& attack, Vector2d source){
	HP -= attack.damage;
	push(pos - source, attack.push);
}

void Player::load(const ConfigNode& config){
	Entity::load(config);
	jumpHeight = config.getDouble("jump-height");
	accelSpeed = config.getDouble("acceleration");
	walkSpeed = config.getDouble("walk-speed");
}
