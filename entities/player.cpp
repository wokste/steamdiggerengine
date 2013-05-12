#include "player.h"
#include <SFML/Window.hpp>
#include "../screen.h"
#include "../attack.h"
#include "../world.h"
#include "../game.h"

#include "../utils/confignode.h"
#include "../items/itemdefmanager.h"

#define STATS ((PlayerStats *)(stats))

Player* PlayerStats::spawn(World* world, Vector2d pos){
	if (!validPos(*world, pos))
		return nullptr;

	auto player = new Player(world, pos, this);
	world->players.push_back(std::unique_ptr<Player>(player));
	return player;
}

Player::Player(World* newWorld, Vector2d newPos, PlayerStats * newStats) : Entity(newWorld, newPos, newStats)
	, inventory(newWorld->game->itemDefs), RP(0){
	//TODO: Fix constructor
}

Player::~Player(){

}

void Player::logic(double time){
	Entity::logic(time);
}

void Player::checkKeyboardMovement(double time){
	bool keyLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
	bool keyRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
	bool slowDown = !(keyLeft ^ keyRight);
	bool moveRight = speed.x > 0;

	if ((keyLeft && !keyRight) || (slowDown && moveRight)){
		speed.x -= (STATS->accelSpeed * time);
	}
	if ((keyRight && !keyLeft) || (slowDown && !moveRight)){
		speed.x += (STATS->accelSpeed * time);
	}

	if (speed.x > STATS->walkSpeed) speed.x = STATS->walkSpeed;
	if (speed.x < -STATS->walkSpeed) speed.x = -STATS->walkSpeed;
	if (speed.x < 0.5 && speed.x > -0.5 && slowDown) speed.x = 0;
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
	if (!STATS->validPos(*world, Vector2d(pos.x, pos.y + 0.1))){
		speed.y = -STATS->jumpHeight;
	}
}

void Player::takeDamage(Attack& attack, Vector2d source){
	HP -= attack.damage;
	push(pos - source, attack.push);
}

void PlayerStats::load(const Game& game, const ConfigNode& config){
	EntityStats::load(game, config);
	jumpHeight = config.getDouble("jump-height");
	accelSpeed = config.getDouble("acceleration");
	walkSpeed = config.getDouble("walk-speed");
}
