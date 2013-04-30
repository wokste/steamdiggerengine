#include "weapon.h"
#include "../entities/player.h"
#include "../entities/projectile.h"

#include "../world.h"
#include "../utils/confignode.h"
#include "../screen.h"
#include "../entities/entity.h"
#include "../game.h"
#include <iostream>

Weapon::Weapon(const Game& game, const ConfigNode& config) : ItemDef(config){
	auto childnode = config.getNodeConst("projectile");
	entitiyType = EntityStats::staticLoad(game, childnode);
}

Weapon::~Weapon(){
	if (entitiyType) delete entitiyType;
}

int Weapon::use(Player& owner, ItemStack& item, const Screen& screen){
	if (!entitiyType)
		return 0;
	Projectile * shot = dynamic_cast<Projectile*>(owner.world->spawn(entitiyType, owner.pos));

	if (shot != nullptr){
		shot->moveTo(screen.mousePos(0));
	}
	return 250;
}
