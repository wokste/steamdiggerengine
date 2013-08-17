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
#include "perlinnoise.h"
#include <cmath>
#include <iostream>

PerlinNoise::PerlinNoise(int newSeed, int newOctaves, double newPersistence, double newScale) :
	seed(newSeed),
	octaves(newOctaves),
	persistence(newPersistence),
	scale(newScale)
{
}

/// This function generates a pseudo-random number based on: x, y and the seed.
/// It is based on prime magic. The return value is in range [-1,1]
double PerlinNoise::random2D(int x,int y) const{
	int n=x+y*57;
	n *= seed;
	n ^= n<<13;
	int nn=(n*(n*n*60493+19990303)+1376312589)&0x7fffffff;
	return 1.0-((double)nn/1073741824.0);
}

/// Interpolation between the given values.
/// Pre: fraction must be between 0 and 1.
double PerlinNoise::interpolate(double value0, double value1, double fraction) const{
	constexpr double PI = 3.1415927;

	//Smooth fraction with cosine
	fraction = (1 - std::cos(fraction * PI)) / 2.0;
	return value0 * (1.0 - fraction) + value1 * (fraction);
}

///This is kinda a cheap way to floor a double integer.
int PerlinNoise::floorInt(double x) const{
  int i = (int)x; // truncate
  return i - ( i > x ); // convert trunc to floor
}

double PerlinNoise::noiseIteration2d(double x,double y) const{
	int xFloored=floorInt(x);
	int yFloored=floorInt(y);
	double topLeft    =random2D(xFloored  ,yFloored);
	double topRight   =random2D(xFloored+1,yFloored);
	double bottomLeft =random2D(xFloored  ,yFloored+1);
	double bottomRight=random2D(xFloored+1,yFloored+1);

	double top   =interpolate(topLeft,   topRight,   x-xFloored);
	double bottom=interpolate(bottomLeft,bottomRight,x-xFloored);
	return        interpolate(top,       bottom,     y-yFloored);
}

double PerlinNoise::noise2d(double x, double y) const{
	x /= scale;
	y /= scale;
	double sum =0;
	for(int i=0;i<octaves-1;i++){
		double frequency = std::pow(0.5,i);
		double amplitude = std::pow(0.5,i);
		sum += noiseIteration2d(x*frequency,y*frequency)*amplitude;
	}
	return sum;
}
