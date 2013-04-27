#include "weapon.h"
#include "../entities/player.h"
#include "../entities/projectile.h"

#include "../world.h"
#include "../utils/confignode.h"
#include "../screen.h"
#include <iostream>

Weapon::Weapon() : projectileType(nullptr){
}

Weapon::~Weapon(){
	if (projectileType) delete projectileType;
}

bool Weapon::use(Player& owner, ItemStack& item, Screen& screen){
	if (!projectileType)
		return false;
	Projectile * shot = dynamic_cast<Projectile*>(owner.world->spawn(projectileType, owner.pos));

	if (shot != nullptr){
		shot->moveTo(screen.mousePos(0));
	}
	return true;
}

void Weapon::load(ConfigNode& config){
	ItemDef::load(config);
	//projectileType = ItemDefs.fromConfig(config.getNode("projectile"));
	//TODO: load projectiles
}
