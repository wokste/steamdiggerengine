#include "vector2.h"
#include <math.h>

Vector2d Vector2::SetLength(Vector2d source, double lengthNew){
	double lengthOld = sqrt(source.x * source.x + source.y * source.y);
	if (lengthOld < 0.1)
		return Vector2d(0,0);

	return source * lengthNew / lengthOld;
}

Vector2d Vector2::iToD(Vector2i source){
	return Vector2d(source.x, source.y);
}

Vector2i Vector2::dToI(Vector2d source){
	Vector2i ret;
	ret.x = (int) (source.x - 0.5);
	ret.y = (int) (source.y - 0.5);
	return ret;
}

Vector2i Vector2::uToI(sf::Vector2u source){
	return Vector2i(source.x, source.y);
}
