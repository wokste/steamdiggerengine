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
    double length(Vector2d source);
    Vector2d iToD(Vector2i source);
    Vector2i dToI(Vector2d source);
    Vector2i uToI(sf::Vector2u source);
    Vector2d normalize(Vector2d source);
}
