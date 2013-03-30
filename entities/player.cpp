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

Player::Player(Vector2d newPos, PlayerStats * stats) : Entity(newPos, stats)
	, inventory(new ItemDefManager()), cooldown(), RP(0){
	entityType = EntityType::ET_Player;
}

Player::~Player(){

}

void Player::logic(int timeMs){
	Entity::logic(timeMs);
	cooldown -= timeMs;
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
	if (speed.x < 10 && speed.x > -10 && slowDown) speed.x = 0;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !validPos(Vector2d(pos.x, pos.y + 4))){
		speed.y = -STATS->jumpHeight;
	}
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

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
		if (HP > 0 && cooldown.done()){
			if (inventory.use(*this, screen.mousePos()))
				cooldown.set(200);
		}
	}
}

void Player::takeDamage(Attack& attack, Vector2d source){
	HP -= attack.damage;
	push(pos - source, attack.push);
}

void PlayerStats::load(ConfigNode& config){
	EntityStats::load(config);
	jumpHeight = config.getInt("jump-height");
	accelSpeed = config.getInt("acceleration");
	walkSpeed = config.getInt("walk-speed");
}
