#include "weapon.h"
#include "../entities/player.h"
#include "../entities/projectile.h"

#include "../world.h"

Weapon::Weapon() : projectileName("bullet.json"){
}

bool Weapon::use(Player& owner, ItemStack& item, Vector2i mousePos){
	Projectile * shot = dynamic_cast<Projectile*>(world->spawn(projectileName, owner.pos));

	if (shot != nullptr){
		shot->moveTo(Vector2::iToD(mousePos));
	}
	return true;
}
