#pragma once
#include <SFML/System/Vector2.hpp>

typedef sf::Vector2i Vector2i;
typedef sf::Vector2<double> Vector2d;

namespace Vector2{
    Vector2d SetLength(Vector2d source, double length);
    Vector2d iToD(Vector2i source);
    Vector2i dToI(Vector2d source);
    Vector2i uToI(sf::Vector2u source);
}
