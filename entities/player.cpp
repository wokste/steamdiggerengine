/*
The MIT License (MIT)

Copyright (c) 2013, Steven Wokke

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "player.h"
#include <SFML/Window.hpp>
#include "../screen.h"
#include "../attack.h"
#include "../world.h"
#include "../game.h"

#include <pugixml.hpp>
#include "../items/itemdefmanager.h"
#include <iostream>

Player::Player(){
	jumpHeight = 0;
	accelSpeed = 0;
	walkSpeed  = 0;
	isPlayer   = true;
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
	} else if (physicsMode == PhysicsMode::Walking) {
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
			HP.cur = HP.max;
			shield.cur = shield.max;
		}
	}

	if (alive()){
		inventory.logic(time);
	}
}

void Player::onMouseWheel(int delta){
	if (delta < 0){
		inventory.selectItem(1, true);
	} else if (delta > 0){
		inventory.selectItem(-1, true);
	}
}

void Player::selectItem(int nr){
	inventory.selectItem(nr, false);
}

bool Player::useItem(Screen& screen){
	if (!alive())
		return false;
	return inventory.use(*this, screen);
}

void Player::tryJump(){
	if (alive() && physicsMode == PhysicsMode::Walking){
		push(Vector2d(0,1), jumpHeight);
	}
}

void Player::load(pugi::xml_node& configNode){
	Creature::load(configNode);
	jumpHeight = configNode.attribute("jump-height").as_double();
	accelSpeed = configNode.attribute("acceleration").as_double();
	walkSpeed = configNode.attribute("walk-speed").as_double();

	team = configNode.attribute("team").as_int(0);
	for (auto& childNode : configNode){
		inventory.add(childNode.attribute("tag").as_string(), childNode.attribute("count").as_int());
	}
}

bool Player::pickupItem(int id, int count){
	return alive() && inventory.add(id,count);
}

void Player::render(const sf::Color& skyColor){
	if (alive())
		Entity::render(skyColor);
}

void Player::takeDamage(const Attack& attack, Vector2d source){
	if (alive()){
		Creature::takeDamage(attack, source);
		if (!alive())
			inventory.dropStuff(0.5, *world, pos);
	}
}
