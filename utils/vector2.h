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
#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include <SFML/Graphics/Rect.hpp>

typedef sf::Vector2i Vector2i;
typedef sf::Vector3i Vector3i;
typedef sf::Vector2<double> Vector2d;

typedef sf::Rect<int> Rect4i;
typedef sf::Rect<double> Rect4d;

namespace Vector2{
    Vector2d setLength(Vector2d source, double newLength);
    double length(Vector2d& source);
    double lengthTo(Vector2d&, Vector2d&);
    Vector2d iToD(Vector2i& source);
    Vector2i floorVec(Vector2d& source);
    Vector2i ceilVec(Vector2d& source);
    Vector2i uToI(sf::Vector2u& source);
    Vector2d normalize(Vector2d& source);
    Vector2d center(Vector2i& source);
    Vector2d center(Vector2d& source);
}
