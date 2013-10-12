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
#include "enums.h"
#include <cassert>
#include <iostream>

BlockCollisionType getBlockCollisionType(std::string name){
	if (name == "air")
		return BlockCollisionType::Air;
	if (name == "solid")
		return BlockCollisionType::Solid;
	if (name == "ladder")
		return BlockCollisionType::Ladder;
	return BlockCollisionType::Air;
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
