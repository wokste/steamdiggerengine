#include "combateffect.h"
#include "../entities/entity.h"
#include "../world.h"
#include "../utils/confignode.h"
#include "../utils/vector2.h"
#include "../entities/projectile.h"

ShootEffect::ShootEffect(const ConfigNode& config){
	projectile.reset(new Projectile());
	projectile->load(config);
}

bool ShootEffect::run(Entity& owner, Vector2d sourcePos, Vector2d targetPos, int targetLayer){
	if (!projectile)
		return false;
	Projectile* shot = owner.world->spawn<Projectile>(*projectile, sourcePos);
	if (shot == nullptr)
		return false;
	shot->targetType = ProjectileTargetType::TargetMonster;
	shot->moveTo(targetPos);
	return true;
}

HealEffect::HealEffect(const ConfigNode& config){
	hpGain = config.getInt("hp",0);
	shieldGain = config.getInt("shield",0);
}

bool HealEffect::run(Entity& owner, Vector2d sourcePos, Vector2d targetPos, int targetLayer){
	auto target = dynamic_cast<Creature*>(&owner);
	bool hasEffect = false;
	if (target){
		hasEffect |= target->HP.heal(hpGain);
		hasEffect |= target->shield.heal(shieldGain);
	}
	return hasEffect;
}
