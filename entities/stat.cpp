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

#include "src/entities/stat.h"

#include <sstream>
#include <cmath>

Stat::Stat(): cur(0), max(0){}
Stat::~Stat(){}

Stat::Stat(int newVal){
	cur = newVal;
	max = newVal;
}

/// returns the amount that is healed.
bool Stat::heal(int healing){
	if (cur == max)
		return false;

	cur = std::min(cur + healing, max);
	return true;
}

/// reduces stat
/// returns the amount that is not soaked.
int Stat::soak(int damage){
	cur -= damage;
	if (cur < 0){
		auto notSoaked = -cur;
		cur = 0;
		return notSoaked;
	}
	return 0;
}

double Stat::asProportion() const{
	if (max <= 0)
		return 1;
	return (double)cur / (double)max;
}

std::string Stat::asText() const{
	std::stringstream out;
	out << cur << " / " << max;
	return out.str();
}
