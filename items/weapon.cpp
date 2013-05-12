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
	projectile = new ProjectileStats();
	projectile->load(game, childnode);
}

Weapon::~Weapon(){
	if (projectile) delete projectile;
}

double Weapon::use(Player& owner, ItemStack& item, const Screen& screen){
	if (!projectile)
		return 0;
	Projectile* shot = projectile->spawn(owner.world, owner.pos);
	shot->targetType = ProjectileTargetType::TargetMonster;

	if (shot != nullptr){
		shot->moveTo(screen.mousePos(0));
		return 0.250;
	}
	return 0;
}
