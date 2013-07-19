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
	jumpHeight = 0;
	accelSpeed = 0;
	walkSpeed  = 0;
}

Player::~Player(){

}

void Player::logic(double time){
	Creature::logic(time);
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
	if (alive()){
		checkKeyboardMovement(time);
	}else{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
			pos.x = 2;
			pos.y = -2;
			HP = 100;
		}
	}

	if (alive()){
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

void Player::load(const ConfigNode& config){
	Creature::load(config);
	jumpHeight = config.getDouble("jump-height");
	accelSpeed = config.getDouble("acceleration");
	walkSpeed = config.getDouble("walk-speed");
}
