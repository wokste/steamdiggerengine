#include "gun.h"
#include "../entities/player.h"
#include "../entities/projectile.h"

#include "../world.h"
#include "../utils/confignode.h"
#include "../screen.h"
#include "../entities/entity.h"
#include "../game.h"
#include <iostream>

Gun::Gun(const Game& game, const ConfigNode& config) : ItemDef(config){
	auto childnode = config.getNodeConst("projectile");
	projectile = new ProjectileStats();
	projectile->load(game, childnode);
}

Gun::~Gun(){
	if (projectile) delete projectile;
}

double Gun::use(Player& owner, ItemStack& item, const Screen& screen){
	if (!projectile)
		return 0;
	Projectile* shot = projectile->spawn(owner.world, owner.pos);
	if (shot == nullptr)
		return 0;
	shot->targetType = ProjectileTargetType::TargetMonster;

	if (shot != nullptr){
		shot->moveTo(screen.mousePos(0));
		return 0.250;
	}
	return 0;
}
