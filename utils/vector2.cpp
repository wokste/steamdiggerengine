#include "vector2.h"
#include <cmath>

Vector2d Vector2::setLength(Vector2d source, double lengthNew){
	double lengthOld = length(source);
	if (lengthOld < 0.1)
		return Vector2d(0,0);

	return source * lengthNew / lengthOld;
}

Vector2d Vector2::normalize(Vector2d source){
	double length = std::hypot(source.x, source.y);
	return (length > 0.01) ? source / length : Vector2d(0,0);
}

double Vector2::length(Vector2d source){
	return std::hypot(source.x, source.y);
}

Vector2d Vector2::iToD(Vector2i source){
	return Vector2d(source.x, source.y);
}

inline int int_floor(double x)
{
  int i = (int)x; /* truncate */
  return i - ( i > x ); /* convert trunc to floor */
}

Vector2i Vector2::dToI(Vector2d source){
	Vector2i ret;
	ret.x = int_floor(source.x);
	ret.y = int_floor(source.y);
	return ret;
}

Vector2i Vector2::uToI(sf::Vector2u source){
	return Vector2i(source.x, source.y);
}
