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
#include "vector2.h"
#include "mathplus.h"
#include <iostream>

Vector2d Vector2::setLength(Vector2d source, double lengthNew){
	double lengthOld = length(source);
	if (lengthOld < 0.1)
		return Vector2d(0,0);

	return source * lengthNew / lengthOld;
}

Vector2d Vector2::normalize(Vector2d& source){
	double length = std::hypot(source.x, source.y);
	return (length > 0.01) ? source / length : Vector2d(0,0);
}

double Vector2::length(Vector2d& source){
	return std::hypot(source.x, source.y);
}

double Vector2::lengthTo(Vector2d& v1, Vector2d& v2){
	return std::hypot(v1.x - v2.x, v1.y - v2.y);
}

Vector2d Vector2::iToD(Vector2i& source){
	return Vector2d(source.x, source.y);
}

Vector2i Vector2::floorVec(Vector2d& source){
	return Vector2i(MathPlus::floorInt(source.x),MathPlus::floorInt(source.y));
}

Vector2i Vector2::ceilVec(Vector2d& source){
	return Vector2i(MathPlus::ceilInt(source.x),MathPlus::ceilInt(source.y));
}

Vector2i Vector2::uToI(sf::Vector2u& source){
	return Vector2i(source.x, source.y);
}

Vector2d Vector2::center(Vector2i& source){
	return Vector2d(source.x + 0.5, source.y + 0.5);
}

Vector2d Vector2::center(Vector2d& source){
	return Vector2d(MathPlus::floorInt(source.x) + 0.5, MathPlus::floorInt(source.y) + 0.5);
}
