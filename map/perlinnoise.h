#pragma once

class PerlinNoise{
public:
	PerlinNoise(int newSeed, int newOctaves, double newPersistence, double newScale = 1);
	double noiseSum(double x, double y) const;
private:
	double rnd2D(int x,int y) const;
	double interpolate(double a,double b,double x) const;
	int int_floor(double x) const;
	double noise2d(double x,double y) const;

	int seed;
	int octaves;
	double persistence;
	double scale;
};
