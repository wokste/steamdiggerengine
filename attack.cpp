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
#include "attack.h"

Attack::Attack() :
	damage(0),
	push(0){
}

void Attack::load(pugi::xml_node& node){
	damageTerrain = node.attribute("damage-terrain").as_bool();

	damage = node.attribute("damage").as_int();
	push = node.attribute("push").as_int();
	for (auto& bonusNode : node){
		AttackBonus bonus;
		bonus.type = bonusNode.attribute("type").as_int();
		bonus.perc = bonusNode.attribute("multiplier").as_float() - 1;
		bonusses.push_back(bonus);
	}
}

float Attack::getBonus(int type){
	float sum = 1;
	for (auto& bonus : bonusses){
		if (type == bonus.type)
			sum += bonus.perc;
	}
}
