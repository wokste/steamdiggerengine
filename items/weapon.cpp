#include "weapon.h"
#include "../entities/player.h"
#include "../entities/projectile.h"

#include "../world.h"
#include "../utils/confignode.h"
#include "../screen.h"
#include <iostream>

Weapon::Weapon() : projectileName(){
}

bool Weapon::use(Player& owner, ItemStack& item, Screen& screen){
	Projectile * shot = dynamic_cast<Projectile*>(owner.world->spawn(projectileName, owner.pos));

	if (shot != nullptr){
		shot->moveTo(screen.mousePos(0));
	}
	return true;
}

void Weapon::load(ConfigNode& config){
	ItemDef::load(config);
	projectileName = config.getString("projectile-name");
}
