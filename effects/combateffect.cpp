/*
The MIT License (MIT)

Copyright (c) 2013, Steven Wokke

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "combateffect.h"
#include "../entities/entity.h"
#include "../world.h"
#include <pugixml.hpp>
#include "../utils/vector2.h"
#include "../entities/projectile.h"

ShootEffect::ShootEffect(pugi::xml_node& node){
	projectile.reset(new Projectile());
	projectile->load(node);
}

int ShootEffect::run(EffectParams& params){
	if (!projectile)
		return 0;
	Projectile* shot = params.entity.world->spawn<Projectile>(*projectile, params.sourcePos);
	if (shot == nullptr)
		return 0;

	if (params.eventInstignator){
		shot->team = params.eventInstignator->team;
	}
	shot->moveTo(params.targetPos);
	return 1;
}

HealEffect::HealEffect(pugi::xml_node& node){
	hpGain = node.attribute("hp").as_int();
	shieldGain = node.attribute("shield").as_int();
}

int HealEffect::run(EffectParams& params){
	int hasEffect = 0;
	auto target = params.eventInstignator;
	if (target){
		hasEffect |= target->HP.heal(hpGain);
		hasEffect |= target->shield.heal(shieldGain);
	}
	return hasEffect;
}
