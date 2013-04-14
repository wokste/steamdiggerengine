#include "player.h"
#include <SFML/Window.hpp>
#include "../screen.h"
#include "../attack.h"

#include "../utils/confignode.h"
#include "../items/itemdefmanager.h"

#define STATS ((PlayerStats *)(stats))

Entity * PlayerStats::spawn(Vector2d newPos){
	return new Player(newPos, this);
}

Player::Player(Vector2d newPos, PlayerStats * newStats) : Entity(newPos, newStats)
	, inventory(new ItemDefManager()), RP(0){
	entityType = EntityType::ET_Player;
}

Player::~Player(){

}

void Player::logic(int timeMs){
	Entity::logic(timeMs);
}

void Player::checkKeyboardMovement(int timeMs){
	bool keyLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
	bool keyRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
	bool slowDown = !(keyLeft ^ keyRight);
	bool moveRight = speed.x > 0;

	if ((keyLeft && !keyRight) || (slowDown && moveRight)){
		speed.x -= ((STATS->accelSpeed * timeMs) / 1000);
	}
	if ((keyRight && !keyLeft) || (slowDown && !moveRight)){
		speed.x += ((STATS->accelSpeed * timeMs) / 1000);
	}

	if (speed.x > STATS->walkSpeed) speed.x = STATS->walkSpeed;
	if (speed.x < -STATS->walkSpeed) speed.x = -STATS->walkSpeed;
	if (speed.x < 0.5 && speed.x > -0.5 && slowDown) speed.x = 0;
}

void Player::checkInput(int timeMs, Screen& screen){
	if (HP > 0){
		checkKeyboardMovement(timeMs);
	}
	else{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
			pos.x = 200;
			pos.y = 200;
			HP = 100;
		}
	}

	if (HP > 0){
		inventory.logic(timeMs, *this, screen);
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

void Player::tryJump(){
	if (!validPos(Vector2d(pos.x, pos.y + 0.1))){
		speed.y = -STATS->jumpHeight;
	}
}

void Player::takeDamage(Attack& attack, Vector2d source){
	HP -= attack.damage;
	push(pos - source, attack.push);
}

void PlayerStats::load(ConfigNode& config){
	EntityStats::load(config);
	jumpHeight = config.getDouble("jump-height");
	accelSpeed = config.getDouble("acceleration");
	walkSpeed = config.getDouble("walk-speed");
}
