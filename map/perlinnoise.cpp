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

double PerlinNoise::rnd2D(int x,int y){
	int n=x+y*57;
	n *= seed;
	n ^= n<<13;
	int nn=(n*(n*n*60493+19990303)+1376312589)&0x7fffffff;
	return 1.0-((double)nn/1073741824.0);
}

double PerlinNoise::interpolate(double a,double b,double x){
	double ft=x * 3.1415927;
	double f=(1.0-std::cos(ft))* 0.5;
	return a*(1.0-f)+b*f;
}

int PerlinNoise::int_floor(double x){
  int i = (int)x; /* truncate */
  return i - ( i > x ); /* convert trunc to floor */
}

double PerlinNoise::noise2d(double x,double y){
	int xI=int_floor(x);//This is kinda a cheap way to floor a double integer.
	int yI=int_floor(y);
	double s,t,u,v;
	s=rnd2D(xI  ,yI);
	t=rnd2D(xI+1,yI);
	u=rnd2D(xI  ,yI+1);
	v=rnd2D(xI+1,yI+1);
	double int1=interpolate(s,t,x-xI);
	double int2=interpolate(u,v,x-xI);
	return interpolate(int1,int2,y-yI);
}

double PerlinNoise::noiseSum(double x, double y){
	x *= scale;
	y *= scale;
	double sum =0;
	for(int i=0;i<octaves-1;i++){
		double frequency = 10;//pow(2,i);
		double amplitude = 1;//pow(persistence,i);
		sum += noise2d(x/frequency,y/frequency)*amplitude;
	}
	return sum;
}
