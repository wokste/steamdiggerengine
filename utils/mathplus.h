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

#include <cmath>

namespace MathPlus{
	///This is kinda a cheap way to floor a double integer.
	inline int floorInt(double x){
		int i = (int)x;
		return i - ( i > x );
	}

	///This is kinda a cheap way to ceil a double integer.
	inline int ceilInt(double x){
		int i = (int)(x);
		return i + ( i < x );
	}

	/// Interpolation between the given values.
	/// Pre: fraction must be between 0 and 1.
	inline double interpolate(double value0, double value1, double fraction){
		constexpr double PI = 3.1415927;

		//Smooth fraction with cosine
		fraction = (1 - std::cos(fraction * PI)) / 2.0;
		return value0 * (1.0 - fraction) + value1 * (fraction);
	}
};
