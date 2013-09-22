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
#include "effect.h"
#include "attackeffect.h"
#include "combateffect.h"
#include "mapeffect.h"

#include <pugixml.hpp>
#include <iostream>

std::unique_ptr<Effect> Effect::loadEffect(pugi::xml_node& node){
	std::unique_ptr<Effect> effect;
	std::string typeName = node.name();

	if (typeName == "attack")
		effect.reset(new AttackEffect(node));
	if (typeName == "shoot")
		effect.reset(new ShootEffect(node));
	if (typeName == "heal")
		effect.reset(new HealEffect(node));

	return effect;
}


EffectSlot::EffectSlot(){

}

EffectSlot::~EffectSlot(){

}

void EffectSlot::load(pugi::xml_node& array){
	for(auto childNode : array){
		add(Effect::loadEffect(childNode));
	}
}

void EffectSlot::add(std::unique_ptr<Effect> newEffect){
	if (newEffect)
		effects.push_back(std::move(newEffect));
}

int EffectSlot::run(EffectParams& params){
	int uses = 0;
	for(auto& effect: effects){
		uses += effect->run(params);
	}
	return uses;
}
