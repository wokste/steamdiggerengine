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
#include <iostream>
#include <sstream>

Attack::Attack() :
	damage(0),
	push(0),
	damageTerrain(false),
	type(DamageType::None)
{
}

void Attack::load(pugi::xml_node& node){
	damageTerrain = node.attribute("damage-terrain").as_bool();

	damage = node.attribute("damage").as_int();
	push = node.attribute("push").as_int();
	type = getDamageType(node.attribute("type").as_string("none"));
}

DamageType getDamageType(std::string name){
	if (name == "none")
		return DamageType::None;
	if (name == "chop")
		return DamageType::Chop;
	if (name == "stab")
		return DamageType::Stab;
	if (name == "mine")
		return DamageType::Mine;
	if (name == "fire")
		return DamageType::Fire;
	if (name == "ice")
		return DamageType::Ice;
	std::cout << "Error: damage type " << name << " is unknown\n";
	return DamageType::None;
}

// PRE: Text formatted like "chop 0.3 stab 1.3"
void Defense::set(std::string text){
	std::stringstream ss(text);
	while (!ss.eof()){
		std::string type;
		std::getline(ss,type, ' ');
		if (type == "")
			return;

		float value;
		ss >> value;
		defenseValues.insert(std::make_pair(getDamageType(type), value));
	}
}

int Defense::getDamage(const Attack& attack){
	auto iterator = defenseValues.find(attack.type);
	if (iterator == defenseValues.end()) {
		return attack.damage;
	} else {
		return iterator->second * attack.damage;
	}
}
