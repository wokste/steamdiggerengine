#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

typedef sf::Vector2i Vector2i;
typedef sf::Vector3i Vector3i;
typedef sf::Vector2<double> Vector2d;

namespace Vector2{
    Vector2d setLength(Vector2d source, double newLength);
    double length(Vector2d source);
    Vector2d iToD(Vector2i source);
    Vector2i dToI(Vector2d source);
    Vector2i uToI(sf::Vector2u source);
}
