#include "weapon.h"
#include "../entities/player.h"
#include "../entities/projectile.h"

#include "../world.h"
#include "../utils/confignode.h"
#include "../screen.h"
#include <iostream>

Weapon::Weapon() : projectileName("bullet.json"){
}

bool Weapon::use(Player& owner, ItemStack& item, Screen& screen){
	std::cout << "used weapon\n";
	Projectile * shot = dynamic_cast<Projectile*>(world->spawn(projectileName, owner.pos));

	if (shot != nullptr){
		shot->moveTo(screen.mousePos(0));
	}
	return true;
}

void Weapon::load(ConfigNode& config){
	std::cout << "loaded weapon\n";
	ItemDef::load(config);
}
