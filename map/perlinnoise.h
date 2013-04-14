#pragma once

class PerlinNoise{
public:
	PerlinNoise(int newSeed, int newOctaves, double newPersistence, double newScale = 1);
	double noiseSum(double x, double y);
private:
	double rnd2D(int x,int y);
	double interpolate(double a,double b,double x);
	int int_floor(double x);
	double noise2d(double x,double y);

	int seed;
	int octaves;
	double persistence;
	double scale;
};
